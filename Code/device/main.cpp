#include "portOpt.h"
#include "include.h"
#include "getmac.h"
#include "mqtt.h"
#include "sensorStruct.h"
#include "printlog.h"
#include "linkDB.h"
#include "CRC.h"
#include "RuleJudger.h"
#include "config.h"
extern "C"
{
#include "coap.h"
}

int com_fd = 0;
MQTTClient client;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
FILE *logfp;
sqlite3 *db;
char *zErrMsg = 0;
char MAC[30]={0};
pthread_mutex_t sendratemutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t configmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t commutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t logmutex = PTHREAD_MUTEX_INITIALIZER;
int GetDataRate = 15;
Json::Value configJson;

void sigfun(int signal)
{
    printlog(WARN, "EXIT","PROGRAM EXIT");
    close(com_fd);
    sqlite3_close(db);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    pthread_mutex_destroy(&sendratemutex);   
    pthread_mutex_destroy(&commutex);   
    pthread_mutex_destroy(&logmutex);   
    exit(0);
}

int main(int argc, char *argv[])
{
    int Txlen,i,ret;
    int tmpData[100],k;
    char sendData[100];
    int RxLen;
    char rxData[200];
    char sub_topic[50]={0};
    char pub_topic[50]={0};
    
    printlog(INFO, "START","PROGRAM START");

    portinfo_t portinfo ={   
        '0',                            // print prompt after receiving   
        9600,                         // baudrate: 9600   
        '8',                            // databit: 8   
        '0',                            // debug: off   
        '0',                            // echo: off   
        '2',                            // flow control: software     
        '0',                            // parity: none   
        '1',                            // stopbit: 1   
        0                          // reserved   
    }; 

    if(argc != 3)
    {
        printf("Wrong argc number,exit!\n");
        exit(1);
    }	

    //open com file
    com_fd = open(argv[1], O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(com_fd<0)
    {
        printf("Open Com failed,exit!\n");
        exit(1);
    }
    
    //??????config   
    if(getConfig()!=0)
    {
        printf("get config failed\n");
    }

    //set sendrate
    GetDataRate = configJson["sendrate"].asInt();
    //???????????? ????????????15
    if(GetDataRate < SendRateMin || GetDataRate > SendRateMax)
    {
        GetDataRate = 15;
        configJson["sendrate"] = GetDataRate;
        setConfig();
    }

    printlog(INFO,"COM","COM file open success");

    //set com
    portinfo.baudrate = 9600;
    PortSet(com_fd, &portinfo);

    //link database
    linkDB(argv[2]);
    
    //get MAC
    get_mac(MAC);

    //link mqtt
    MQTTClient_create(&client, ADDRESS, MAC, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 60;
    conn_opts.cleansession = 1;
    
    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    
    printlog(INFO,"MQTT","MQTT connect success");

    //??????????????????
    sprintf(pub_topic,"%s",MAC);

    //??????????????????
    sprintf(sub_topic,"%s_service",MAC);

    //???????????????????????????
    pthread_t receiver;
    if(pthread_create(&receiver,NULL,subMessage,(void *)sub_topic) != 0)
    {
        printf("sub thread creation failed!\n");
        exit(1);
    }
    printlog(INFO,"MQTT","MQTT sub success");
    
    //??????coap
    pthread_t coapThread;
    if(pthread_create(&coapThread,NULL,runCoap,NULL) != 0)
    {
        printf("coap thread creation failed!\n");
        exit(1);
    }

    //??????????????????trigerState??????
    sendSQL((char*)"update rules set trigerState=0;");
    
    signal(SIGINT,sigfun);
    
    printf("**********Program running**********\n\n");

    //starting get data
    time_t oldt, newt;
    while(1)
    {
        //???????????? ??????????????????
        oldt = time(NULL);

        cout<<"------start to get data------"<<endl;

        cout<<"MAC: "<<MAC<<endl;
        cout<<"Time: "<<oldt<<endl;

        Json::Value sendJson;
        sendJson["MAC"] = MAC;
        sendJson["Time"] = (int)oldt;

        sqlite3_stmt *stmt;
        int res;

        const char *selectSql = "select addr,code_485,returnLength from sensors where state=1 order by addr;";
        res = sqlite3_prepare_v2(db,selectSql,strlen(selectSql),&stmt,0);
        if(res != SQLITE_OK)
        {
            printf("sqlite3_prepare_v2 error\n");
            //fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));
            //log

            continue;
        }
        //3
        res = sqlite3_step(stmt);
        /*
           if(res != SQLITE_DONE)
           {
           printf("sqlite3_step error\n");
           return -1;
           }*/

        int count = sqlite3_column_count(stmt);
        //printf("count = %d\n",count);
        /*
           int i;
        //??????????????????????????????
        for(i = 0; i < count; i++)
        {
        printf("column name:%s\n",sqlite3_column_name(stmt,i));
        printf("column type:%i\n",sqlite3_column_type(stmt,i));
        }*/


        //??????sensor??????
        vector<sensor> activeSensors;

        //??????json??????
        Json::Value Data_json;

        //???????????????
        int addr;
        const unsigned char *code_485;
        int returnLength;
        while(res == SQLITE_ROW)
        {
            addr = sqlite3_column_int(stmt,0);
            code_485 = sqlite3_column_text(stmt,1);
            returnLength = sqlite3_column_int(stmt,2);
            
            sensor temp;
            temp.addr = addr;

            temp.code_485 = (char*)code_485;

            temp.returnLength = returnLength;

            temp.state = 1;
            activeSensors.push_back(temp);

            res = sqlite3_step(stmt);
        }
        //4
        sqlite3_finalize(stmt);

        printf("I am reading sensor data");
        fflush(stdout);

        //for ??????sensrs ??????addr 485?????? returnLength

        //?????????????????????
        for(int sensorIndex=0; 
                sensorIndex<activeSensors.size(); 
                sensorIndex++)
        {
            //?????????Json
            Json::Value sensor_json;
            sensor_json["addr"] = activeSensors[sensorIndex].addr;
            sensor_json["code_485"] = activeSensors[sensorIndex].code_485;

            //?????????code_485 ?????????unsigned char??????
            string str = activeSensors[sensorIndex].code_485;
            string tmp;
            Txlen = 0;

            //string to bytes
            Txlen = codeTransfer(str, sendData);

            int SensorNum = (int)sendData[0];
            int optNum = (int)sendData[1];

            //??????485??????????????? ??????returnlength

            int gotCount = 0;
            int timeoutFlag = 0, selectError = 0;
            int needlen = activeSensors[sensorIndex].returnLength;
            char allGotData[200] = {0};

            ret = SendCode(com_fd, sendData, Txlen, needlen, allGotData);
            if(ret != SensorGetDataOK)
            {
                if(ret == SensorTimeOut)
                {
                    printf("senser %02x time out! check the sensor or your code!\n",activeSensors[sensorIndex].addr);
                }
                else if(ret == SensorSelectFailed)
                {
                    printf("sensor %02x select < 0! check the sensor!\n",activeSensors[sensorIndex].addr)  ;  
                }
                else if(ret == SensorGotWrongData)
                {
                    printf("Got wrong data! check your code or rule!");    
                }
                continue;
            }
            //usleep(500000);

            //----------??????????????????????????????  ?????????????????????485----
            if(SensorNum != allGotData[0] ||
                    optNum != allGotData[1])
            {
                printf("received wrong data!\n");
                continue;
            }

            //???addr???type???(order by byteStart) ??????
            char selectType[100];
            sprintf(selectType, "select * from types where addr=%d;",activeSensors[sensorIndex].addr);
            res = sqlite3_prepare_v2(db,selectType,strlen(selectType),&stmt,0);
            if(res != SQLITE_OK)
            {
                printf("sqlite3_prepare_v2 error\n");
                //fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));
                //log

                continue;
            }
            //3
            res = sqlite3_step(stmt);

            int typesCount = sqlite3_column_count(stmt);

            //??????types??????
            vector<sensor_type> sensorTypes;

            //???????????????
            int addr;
            const unsigned char *typeName;
            int byteStart;
            int byteCount;
            const unsigned char *dataDefine;
            int dicimal;
            int negative;

            while(res == SQLITE_ROW)
            {
                addr = sqlite3_column_int(stmt,0);
                typeName = sqlite3_column_text(stmt,1);
                byteStart = sqlite3_column_int(stmt,2);
                byteCount = sqlite3_column_int(stmt,3);
                dataDefine = sqlite3_column_text(stmt,4);
                dicimal = sqlite3_column_int(stmt,5);
                negative = sqlite3_column_int(stmt,6);

                sensor_type temp;

                temp.addr = addr;
                temp.typeName = (char*)typeName;
                temp.byteStart = byteStart;
                temp.byteCount = byteCount;
                temp.dataDefine = (char*)dataDefine;
                temp.dicimal = dicimal;
                temp.negative = negative;

                sensorTypes.push_back(temp);

                res = sqlite3_step(stmt);
            }
            sqlite3_finalize(stmt);

            //??????????????????????????????types
            Json::Value Data;
            for(int typeIndex=0;
                    typeIndex < sensorTypes.size();
                    typeIndex++)
            {
                //gotCount ?????? allGotData[]
                //????????????
                int start = sensorTypes[typeIndex].byteStart;
                int len = sensorTypes[typeIndex].byteCount;
                int dicimal = sensorTypes[typeIndex].dicimal;
                int negative = sensorTypes[typeIndex].negative;
                string typeName = sensorTypes[typeIndex].typeName;
                string dataDefine = sensorTypes[typeIndex].dataDefine;

                std::stringstream stringStream;
                //??????????????????
                string resStr;

                if(dataDefine.compare("int") == 0)
                {
                    int res = 0;
                    for(int i=start; i < start+len; i++)
                    {
                        res = res << 8;
                        res += (int)allGotData[i];
                    }
                    if(negative == 1)
                    {
                        if(len == 2)
                        {
                            res = (short)res;
                        }
                        else if(len == 4)
                        {
                            res = (int)res;   
                        }
                    }

                    stringStream << res;
                    Data[typeName] = stringStream.str();
                    resStr = stringStream.str();
                    //cout<<typeName+": "<<res<<endl;
                }
                else if(dataDefine.compare("float") == 0)
                {
                    float res = 0;
                    int temp = 0;
                    for(int i=start; i < start+len; i++)
                    {
                        temp = temp << 8;
                        temp += (int)allGotData[i];
                    }

                    if(negative == 1)
                    {
                        if(len == 2)
                        {
                            temp = (short)temp;
                        }
                        else if(len == 4)
                        {
                            temp = (int)temp;   
                        }
                    }
                    
                    res = temp;

                    for(int i=0; i<dicimal; i++)
                    {
                        res /= 10.0;    
                    }
                    stringStream << res;
                    Data[typeName] = stringStream.str();
                    resStr = stringStream.str();
                    //cout<<typeName+": "<<res<<endl;
                }
                else if(dataDefine.compare("double") == 0)
                {
                    double res = 0;
                    int temp = 0;
                    for(int i=start; i < start+len; i++)
                    {
                        temp = temp << 8;
                        temp += (int)allGotData[i];
                    }

                    if(negative == 1)
                    {
                        if(len == 2)
                        {
                            temp = (short)temp;
                        }
                        else if(len == 4)
                        {
                            temp = (int)temp;   
                        }
                    }
                    
                    res = temp;
                    
                    for(int i=0; i<dicimal; i++)
                    {
                        res/=10.0;    
                    }
                    stringStream << res;
                    Data[typeName] = stringStream.str();
                    resStr = stringStream.str();
                    //cout<<typeName+": "<<res<<endl;
                }
                else if(dataDefine.compare("string") == 0)
                {
                    string res = "";
                    for(int i=start; i < start+len; i++)
                    {
                        res+=(char)allGotData[i];
                    }
                    Data[typeName] = res;
                    resStr = res;
                    //cout<<typeName+": "<<res<<endl;   
                }

                //rule judge ???????????????string
                if(dataDefine.compare("string") != 0)
                {
                    int addr = sensorTypes[typeIndex].addr;
                    //??????????????????????????????
                    int ret = RuleJudger(addr,typeName,resStr);
                }

            }
            //sensor types????????????
            sensor_json["Data"] = Data;
            Data_json.append(sensor_json);
            printf(".");
            fflush(stdout);

        }
        printf("\n");
        //activeSensors????????????
        sendJson["Sensors"] = Json::Value(Data_json);
        sendJson["sendRate"] = GetDataRate;
        //sendJson????????????

        //debug: ???????????????????????????json
        cout<<sendJson.toStyledString()<<endl;

        char temp[2048]={0};
        Json::FastWriter writer;

        strcpy(temp, writer.write(sendJson).c_str());
        //cout << temp<<endl;

        if(pubMessage(pub_topic, temp) != MQTTCLIENT_SUCCESS)
        {
            printf("pub failed\n");
            //log
        }

        cout<<"------------waiting send rate-------------"<<endl;
        printf("\n");

        //newt = oldt;
        newt = time(NULL);
        
        pthread_mutex_lock(&sendratemutex);
        int rateTemp = GetDataRate;
        pthread_mutex_unlock(&sendratemutex);

        while(newt < oldt + rateTemp)
        {
            pthread_mutex_lock(&sendratemutex);
            rateTemp = GetDataRate;
            pthread_mutex_unlock(&sendratemutex);
            
            usleep(1000);
            newt = time(NULL);
        }
    }
}

