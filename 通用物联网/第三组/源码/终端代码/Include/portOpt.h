#ifndef __portopt__
#define __portopt__
#include "include.h"
using namespace std;
//COM struct
typedef struct{
	char    prompt;         //prompt after reciving data
	int     baudrate;               //baudrate
	char    databit;                //data bits, 5, 6, 7, 8
	char    debug;          //debug mode, 0: none, 1: debug
	char    echo;                   //echo mode, 0: none, 1: echo
	char    fctl;                   //flow control, 0: none, 1: hardware, 2: software
	char    parity;         //parity 0: none, 1: odd, 2: even
	char    stopbit;                //stop bits, 1, 2
	const int reserved;     //reserved, must be zero
}portinfo_t;
typedef portinfo_t *pportinfo_t;

//boud transfer 
int convbaud(unsigned long int baudrate);

int PortSet(int fdcom, const pportinfo_t pportinfo);

int PortSend(int fdcom, char *data, int datalen);

int PortRecv(int fdcom, char *data, int datalen);

void PortRecvLast(int fdcom);

int SendCode(int com_fd, char *sendData,int Txlen, int needlen, char *allGotData);

int DebugSendCode(int com_fd, char *sendData,int Txlen, char *allGotData);

int codeTransfer(string code_485, char *sendData);

#endif

