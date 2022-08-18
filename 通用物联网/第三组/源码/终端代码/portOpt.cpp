#include"include.h"
#include"portOpt.h"

using namespace std;

//transfer COM
int convbaud(unsigned long int baudrate)
{
    switch(baudrate){
        case 2400:
            return B2400;
        case 4800:
            return B4800;
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        default:
            return B9600;
    }
}

//setup COM
//fdcom 串口描述符 ,pportinfo 待设置的端口信息
int PortSet(int fdcom, const pportinfo_t pportinfo)
{
    struct termios termios_old, termios_new;
    int     baudrate, tmp;
    char    databit, stopbit, parity, fctl;

    memset(&termios_old, 0, sizeof(termios_old));
    memset(&termios_new, 0, sizeof(termios_new));
    cfmakeraw(&termios_new);
    tcgetattr(fdcom, &termios_old);		//get the serial port attributions
    //baudrates
    baudrate = convbaud(pportinfo -> baudrate);
    cfsetispeed(&termios_new, baudrate);	//in boudrate
    cfsetospeed(&termios_new, baudrate);	//out boudrate
    termios_new.c_cflag |= CLOCAL;		//不占用端口
    termios_new.c_cflag |= CREAD;		//能够读取端口

    //控制模式 flow control
    fctl = pportinfo-> fctl;
    switch(fctl){
        case '0':{
                     termios_new.c_cflag &= ~CRTSCTS;	//no flow control
                 }break;
        case '1':{
                     termios_new.c_cflag |= CRTSCTS;     //hardware
                 }break;
        case '2':{
                     termios_new.c_iflag |= IXON | IXOFF |IXANY; //software flow control
                 }break;
    }

    //data bits
    termios_new.c_cflag &= ~CSIZE; 
    databit = pportinfo -> databit;
    switch(databit){
        case '5':
            termios_new.c_cflag |= CS5;
        case '6':
            termios_new.c_cflag |= CS6;
        case '7':
            termios_new.c_cflag |= CS7;
        default:
            termios_new.c_cflag |= CS8;
    }

    //parity check
    parity = pportinfo -> parity;
    switch(parity){
        case '0':{
                     termios_new.c_cflag &= ~PARENB;     //no parity check
                 }break;
        case '1':{
                     termios_new.c_cflag |= PARENB;      //odd check
                     termios_new.c_cflag &= ~PARODD;
                 }break;
        case '2':{
                     termios_new.c_cflag |= PARENB;      //even check
                     termios_new.c_cflag |= PARODD;
                 }break;
    }

    //stop bits
    stopbit = pportinfo -> stopbit;
    if(stopbit == '2'){
        termios_new.c_cflag |= CSTOPB;  //2 stop bits
    }
    else{
        termios_new.c_cflag &= ~CSTOPB; //1 stop bits
    }

    //other attributions default
    termios_new.c_oflag &= ~OPOST;	//原始数据输出
    termios_new.c_cc[VMIN]  = 1;    //控制字符，所要读取字符的最小数量
    termios_new.c_cc[VTIME] = 1;	//读取第一个字符的等待时间

    tcflush(fdcom, TCIFLUSH);     //溢出数据可以接收，但不读
    //设置新属性 所有改变立即生效
    tmp = tcsetattr(fdcom, TCSANOW, &termios_new);
    return(tmp);
}

// 返回实际发送长度
int PortSend(int fdcom, char *data, int datalen)
{
    int len = 0;

    len = write(fdcom, data, datalen);
    if(len == datalen){
        return (len);
    }
    else{
        tcflush(fdcom, TCOFLUSH);
        return -1;
    }
}

//返回实际读入的字节数
int PortRecv(int fdcom, char *data, int datalen)
{
    int readlen, fs_sel;
    fd_set  fs_read;
    struct timeval tv_timeout;

    FD_ZERO(&fs_read);
    FD_SET(fdcom, &fs_read);
    tv_timeout.tv_sec = 1;
    tv_timeout.tv_usec = 500000;

    fs_sel = select(fdcom+1, &fs_read, NULL, NULL, &tv_timeout);
    if(fs_sel > 0){
        readlen = read(fdcom, data, datalen);
        return (readlen);
    }

    return fs_sel;
}

//清除COM口多余的数据
void PortRecvLast(int fdcom)
{
    int readlen, fs_sel;
    fd_set  fs_read;
    struct timeval tv_timeout;
    char data[100];
    int datalen = 100;

    while(1)
    {
        FD_ZERO(&fs_read);
        FD_SET(fdcom, &fs_read);
        tv_timeout.tv_sec = 0;
        tv_timeout.tv_usec = 20000;

        fs_sel = select(fdcom+1, &fs_read, NULL, NULL, &tv_timeout);
        if(fs_sel > 0){
            readlen = read(fdcom, data, datalen);
        }
        if(fs_sel <= 0)
            break;
    }
}

int SendCode(int com_fd, char *sendData, int Txlen, int needlen, char *allGotData)
{
    int i,RxLen;
    //发送485指令并获取 比对returnlength
    
    //加锁
    pthread_mutex_lock(&commutex);
    
    //发送之前清空
    PortRecvLast(com_fd);
    PortSend(com_fd,sendData,Txlen);

    //printf("Receive data in Hex:\n");

    int gotCount = 0;
    char rxData[200];

    while(gotCount < needlen)
    {
        RxLen = PortRecv(com_fd,rxData,sizeof(rxData));
        if(RxLen > 0)
        {
            for(i=0;i<RxLen;i++)
            {
                allGotData[gotCount++] = rxData[i];
                //printf("%02x ",rxData[i]);
            }
            //printf("\n");
        }
        else if(RxLen == 0){
            if(gotCount == 0)
            {
                usleep(COMLOCKTIME);
                pthread_mutex_unlock(&commutex);
                return SensorTimeOut;
            }
            else
            {
                break;    
            }
        }
        else if(RxLen < 0){
            usleep(COMLOCKTIME);
            pthread_mutex_unlock(&commutex);
            return SensorSelectFailed;
        }
    }
    
    usleep(COMLOCKTIME);
    pthread_mutex_unlock(&commutex);

    if(gotCount != needlen)
    {
        //got too much
        if(gotCount > needlen)
            printf("got too much data!\n");
        else
            printf("got data not enough!\n");
        return SensorGotWrongData;
    }

    return SensorGetDataOK;
}

//调试专用接口 返回实际获取的长度
int DebugSendCode(int com_fd, char *sendData, int Txlen, char *allGotData)
{
    int i,RxLen;
    //发送485指令并获取 比对returnlength
    
    //加锁
    pthread_mutex_lock(&commutex);
    
    //发送之前清空
    PortRecvLast(com_fd);
    PortSend(com_fd,sendData,Txlen);

    //printf("Receive data in Hex:\n");

    int gotCount = 0;
    char rxData[200];

    while(1)
    {
        RxLen = PortRecv(com_fd,rxData,sizeof(rxData));
        if(RxLen > 0)
        {
            for(i=0;i<RxLen;i++)
            {
                allGotData[gotCount++] = rxData[i];
            }
        }
        else if(RxLen == 0){
            //超时 返回读取长度
            usleep(COMLOCKTIME);
            pthread_mutex_unlock(&commutex);
            return gotCount;
        }
        else if(RxLen < 0){
            usleep(COMLOCKTIME);
            pthread_mutex_unlock(&commutex);
            return -1;
        }
    }
    
    usleep(COMLOCKTIME);
    pthread_mutex_unlock(&commutex);
    return -2;
}

int codeTransfer(string code_485, char *sendData)
{
    string tmp;
    int Txlen=0;
    for(int i=0;i<code_485.length();i+=3)
    {
        tmp = code_485.substr(i,2);
        int a = (int)strtol((char*)tmp.c_str(),NULL,16);
        sendData[Txlen++]=(unsigned char)a;
    }
    return Txlen;
}

