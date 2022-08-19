#include "include.h"
#include "mqtt.h"
#include "portOpt.h"
#include "CRC.h"
#include "BASE64.h"
#include "config.h"

using namespace std;

//MQTTClient client;
//MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

int IsActiveSensor(int addr)
{
    sqlite3_stmt *stmt;
    int res;
    char selectSql[100]={0};
    sprintf(selectSql,"select state from sensors where addr = %d;",addr);
    res = sqlite3_prepare_v2(db,selectSql,(size_t)strlen(selectSql),&stmt,0);
    if(res != SQLITE_OK)
    {
        printf("[IsActiveSensor] sqlite3_prepare_v2 error\n");
        sqlite3_finalize(stmt);
        return SQLERROR;
    }
    res = sqlite3_step(stmt);
    int count = sqlite3_column_count(stmt);
    if(res != SQLITE_ROW)
    {
        printf("[IsActiveSensor] Null addr\n");
        sqlite3_finalize(stmt);
        return NullAddr;
    }
    int state;
    state = sqlite3_column_int(stmt,0);
    if(state == 0)
    {
        printf("[IsActiveSensor] Not Active Sensor\n");
        sqlite3_finalize(stmt);
        return NotActiveSensor;    
    }
    else if(state == 1)
    {
        printf("[IsActiveSensor] Is Active Sensor\n");
        sqlite3_finalize(stmt);
        return ActiveSensor;
    }
    return -100;
}

// return 0 success ,-1 failed
int setSensorState(int addr, int state)
{
    int res;
    char selectSql[100]={0};
    char *szErrMsg;

    sprintf(selectSql,"update sensors set state=%d where addr=%d;",state,addr);
    //printf("[set state to on] prepare to sql :%s\n",selectSql);
    res = sqlite3_exec(db,selectSql, NULL, NULL, &szErrMsg);
    if(res != SQLITE_OK)
    {
        printf("[%s] %s\n",__FUNCTION__,szErrMsg);
        sqlite3_free(szErrMsg);
        return -1;
    }
    sqlite3_free(szErrMsg);

    return 0;
}

int sendSQL(char *sql)
{
    int res;
    char *szErrMsg;

    printf("[%s] prepare to sql :%s\n",__FUNCTION__,sql);
    res = sqlite3_exec(db,sql, NULL, NULL, &szErrMsg);
    if(res != SQLITE_OK)
    {
        printf("[%s] %s\n",__FUNCTION__,szErrMsg);
        sqlite3_free(szErrMsg);
        return -1;
    }
    sqlite3_free(szErrMsg);

    return 0;    
}

bool IsJsonIllegal(const char *jsoncontent)
{
    std::stack<char> jsonstr;
    const char *p = jsoncontent;
    const char *k = jsoncontent;
    char startChar = jsoncontent[0];
    char endChar = '\0';
    bool isObject = false;//防止 {}{}的判断
    bool isArray = false;//防止[][]的判断

    while (*(k+1) != '\0')
    {
        if(*k == ':' && *(k+1) !='"' && *(k+1) != '[')
        {
            return false;
        }
        k++;
    }

    while (*p != '\0')
    {
        endChar = *p;
        switch (*p)
        {
            case '{':
                if (!isObject)
                {
                    isObject = true;
                }
                else  if (jsonstr.empty())//对象重复入栈
                {
                    return false;
                }
                jsonstr.push('{');
                break;
            case '"':
                if (jsonstr.empty() || jsonstr.top() != '"')
                {
                    jsonstr.push(*p);
                }
                else
                {
                    jsonstr.pop();
                }
                break;
            case '[':
                if (!isArray)
                {
                    isArray = true;
                }
                else  if (jsonstr.empty())//数组重复入栈
                {
                    return false;
                }
                jsonstr.push('[');
                break;
            case ']':
                if (jsonstr.empty() || jsonstr.top() != '[')
                {
                    return false;
                }
                else
                {
                    jsonstr.pop();
                }
                break;
            case '}':
                if (jsonstr.empty() || jsonstr.top() != '{')
                {
                    return false;
                }
                else
                {
                    jsonstr.pop();
                }
                break;
            case '\\'://被转义的字符,跳过
                p++;
                break;
            default:
                ;
        }
        p++;
    }

    if (jsonstr.empty())
    {
        //确保是对象或者是数组,之外的都不算有效 
        switch (startChar)//确保首尾符号对应
        {
            case  '{':
                {
                    if (endChar = '}')
                    {
                        return true;
                    }
                    return false;
                }
            case '[':
                {
                    if (endChar = ']')
                    {
                        return true;
                    }
                    return false;
                }
            default:
                return false;
        }

        return true;
    }
    else
    {
        return false;
    }

}

string sendrate_opt(string operation,string content)
{
    std::stringstream stringStream;
    stringStream << content;
    int sendrate;
    stringStream >> sendrate;

    if(sendrate < SendRateMin || sendrate > SendRateMax)
    {
        char detail[100]={0};
        sprintf(detail,"someone wanted to set sendrate to %d and failed",sendrate);
        printlog(INFO, "control", detail);
        return "sendrate is not available!\n";
    }

    if(operation.compare("setSendrate") == 0)
    {
        //set sendrate in lock
        pthread_mutex_lock(&sendratemutex);
        GetDataRate = sendrate;
        pthread_mutex_unlock(&sendratemutex);
        
        //set config
        pthread_mutex_lock(&configmutex);
        configJson["sendrate"] = sendrate;
        setConfig();
        pthread_mutex_unlock(&configmutex);

        //cout << root["operation"].asString() << endl;
        //cout << root["content"].asString() << endl;
        printf("set sendrate to %d\n",sendrate);
        
        char detail[100]={0};
        sprintf(detail,"sendrate is setted to %d",sendrate);
        printlog(INFO, "control",detail);
        return "success";
    }
    else
    {
        return "payload_error";
    }
}

string doa_opt(string operation,string content)
{
    std::stringstream stringStream;
    stringStream << content;
    unsigned short op;
    stringStream >> op;

    if(operation.compare("Setperiod") == 0)
    {
        //修改清洗周期
        //printf("setperiod\n");
        //cout << root["content"].asString() << endl;
        unsigned char temp[20]={0x05,0x06,0x00,0x0e,0x00,0x01};
        char gotData[50];
        int len = 6, needlen = 8, ret;

        if(op < 5 || op > 999)
        {
            printf("[%s]this data is not available\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"someone wanted to set doa period to %d and failed",op);
            printlog(INFO, "control", detail);
            return "this data is not available!";
        }

        temp[4] = op >> 8;
        temp[5] = op & 0x00ff;

        CRC_Calculater(temp, len);

        len+=2;

        ret = SendCode(com_fd,(char*)temp,len,needlen,gotData);
        if(SensorGetDataOK != ret)
        {
            if(ret == SensorTimeOut)
            {
                printf("[%s] sensor time out! check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor time out",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor time out! check the sensor!";
            }
            else if(ret == SensorSelectFailed)
            {    
                printf("[%s] sensor select error!check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor select error",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor select error!check the sensor!";
            }
            else if(ret == SensorGotWrongData)
            {
                printf("[%s] got wrong data, maybe sensor is unavailable\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor got wrong data",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "got wrong data, maybe sensor is unavailable";
            }
        }

        char detail[100]={0};
        sprintf(detail,"the period of DOA is setted to %d",op);
        printlog(INFO, "control",detail);
        printf("[%s] ctrl_success\n",operation.c_str());
        return "success";
    }
    else if(operation.compare("Setduration") == 0)
    {
        printf("Setduration\n");
        //cout << root["operation"].asString() << endl;
        //cout << root["content"].asString() << endl;
        return "this function is not available now\n";
    }
    else
    {
        return "payload_error";
    }
}

string sensor_opt(Json::Value root)
{
    string operation = root["operation"].asString();

    if(operation.compare("Adddefault") == 0)
    {
        //printf("Adddefault\n");
        int addr = atoi(root["content"].asString().c_str());
        int ret = IsActiveSensor(addr);
        if(ret == ActiveSensor)
        {
            char detail[100]={0};
            sprintf(detail,"add default sensor (%d) success, but it's already active",addr);
            printlog(INFO, "control", detail);
            return "success";
        }
        else if(ret == NotActiveSensor)
        {
            //查询一次数据 验证是否能够成功

            //查询sensor信息
            char selectSql[100] = {0};
            int res, returnLength;
            string code_485;
            sqlite3_stmt *stmt;

            sprintf(selectSql,"select code_485,returnLength from sensors where addr=%d",addr);
            res = sqlite3_prepare_v2(db,selectSql,strlen(selectSql),&stmt,0);
            if(res != SQLITE_OK)
            {
                printf("sqlite3_prepare_v2 error\n");
                //fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));
                //log
                char detail[100]={0};
                sprintf(detail,"[%s] sql error",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "failed, sql error";
            }
            res = sqlite3_step(stmt);

            code_485 = (char*)sqlite3_column_text(stmt,0);
            returnLength = sqlite3_column_int(stmt,1);

            //构造发送指令
            string tmpstr;
            unsigned char temp[20] = {0};
            int len=0, ret;
            char gotData[200]={0};

            //可以改为函数
            for(int i=0; i<code_485.size(); i+=3)
            {
                tmpstr = code_485.substr(i,2);
                int a = (int)strtol((char*)tmpstr.c_str(),NULL,16);
                temp[len++] = (unsigned char)a;
            }

            //向传感器发送指令
            ret = SendCode(com_fd,(char*)temp,len,returnLength,gotData);
            if(SensorGetDataOK != ret)
            {
                if(ret == SensorTimeOut)
                {
                    printf("[%s] sensor time out! check the sensor!\n",operation.c_str());
                    char detail[100]={0};
                    sprintf(detail,"[%s] sensor time out",__FUNCTION__);
                    printlog(WARN, "control", detail);
                    return "sensor time out! check the sensor or your code!";
                }
                else if(ret == SensorSelectFailed)
                {    
                    printf("[%s] sensor select error!check the sensor!\n",operation.c_str());
                    char detail[100]={0};
                    sprintf(detail,"[%s] sensor select error",__FUNCTION__);
                    printlog(WARN, "control", detail);
                    return "sensor select error!check the sensor!";
                }
                else if(ret == SensorGotWrongData)
                {
                    printf("[%s] got wrong data, maybe sensor is unavailable\n",operation.c_str());
                    char detail[100]={0};
                    sprintf(detail,"[%s] sensor got wrong data",__FUNCTION__);
                    printlog(WARN, "control", detail);
                    return "got wrong data, check yoursensor";
                }
            }

            //set state to on    
            res = setSensorState(addr,1);
            if(res == 0)
            {
                char detail[100]={0};
                sprintf(detail,"sensor %d turn on",addr);
                printlog(INFO, "control",detail);
                return "success";   
            }
            else
            {
                char detail[100]={0};
                sprintf(detail,"fail to turn on the sensor");
                printlog(INFO, "control", detail);
                return "fail to turn on the sensor";
            }

        }
        else if(ret == NullAddr)
        {
            char detail[100]={0};
            printf("before\n");
            sprintf(detail,"turn on failed, this addr hasn't been used");
            printlog(INFO, "control", detail);
            return "this addr hasn't been used";    
        }
        else if(ret == SQLERROR)
        {
            char detail[100]={0};
            sprintf(detail,"[%s] sql error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "failed, sql error";    
        }
        char detail[100]={0};
        sprintf(detail,"[%s] failed, unknown error",__FUNCTION__);
        printlog(WARN, "control", detail);
        //cout << root["operation"].asString() << endl;
        //cout << root["content"].asString() << endl;
        return "failed, unknown wrong";    
    }
    else if(operation.compare("Addcustomtest") == 0)
    {
        printf("addcustomtest\n");
        int addr = atoi(root["addr"].asString().c_str());
        int returnLength = atoi(root["returnLength"].asString().c_str());
        string code_485 = root["code485"].asString();
        string tmpstr;
        unsigned char temp[20] = {0};
        int len=0, ret;
        char gotData[200]={0};
        string retStr = "";

        if(addr <= 10)
        {
            printf("[%s] wrong addr\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"addr %d can not be changed",addr);
            printlog(INFO, "control", detail);
            return "this addr can not be changed!";
        }

        for(int i=0; i<code_485.size(); i+=3)
        {
            tmpstr = code_485.substr(i,2);
            int a = (int)strtol((char*)tmpstr.c_str(),NULL,16);
            temp[len++] = (unsigned char)a;
        }

        //向传感器发送指令
        ret = SendCode(com_fd,(char*)temp,len,returnLength,gotData);
        if(SensorGetDataOK != ret)
        {
            if(ret == SensorTimeOut)
            {
                printf("[%s] sensor time out! check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor time out",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor time out! check the sensor or your code!";
            }
            else if(ret == SensorSelectFailed)
            {    
                printf("[%s] sensor select error!check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor select error",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor select error!check the sensor!";
            }
            else if(ret == SensorGotWrongData)
            {
                printf("[%s] got wrong data, maybe sensor is unavailable\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor got wrong data",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "got wrong data, check your code and returnLength or yoursensor";
            }
        }

        //获取到了正确数据 开始解析
        for(unsigned int i=0; i<root["types"].size(); i++)
        {
            string typeName = root["types"][i]["typeName"].asString();    
            int start = atoi(root["types"][i]["byteStart"].asString().c_str());
            int len = atoi(root["types"][i]["byteCount"].asString().c_str());
            string dataDefine = root["types"][i]["dataDefine"].asString();
            int dicimal = atoi(root["types"][i]["dicimal"].asString().c_str());
            int negative = atoi(root["types"][i]["negative"].asString().c_str());

            std::stringstream stringStream;

            if(dataDefine.compare("int") == 0)
            {
                int res = 0;
                for(int i=start; i < start+len; i++)
                {
                    res *= 0x100;
                    res += (int)gotData[i];
                }
                stringStream << res;
                retStr += typeName + ": " + stringStream.str() + ", ";
                //cout<<typeName+": "<<res<<endl;
            }
            else if(dataDefine.compare("float") == 0)
            {
                float res = 0;
                for(int i=start; i < start+len; i++)
                {
                    res *= 0x100;
                    res += (int)gotData[i];
                }
                for(int i=0; i<dicimal; i++)
                {
                    res /= 10.0;    
                }
                stringStream << res;
                retStr += typeName + ": " + stringStream.str() + ", ";
                //cout<<typeName+": "<<res<<endl;
            }
            else if(dataDefine.compare("double") == 0)
            {
                double res=0;
                for(int i=start; i < start+len; i++)
                {
                    res *= 0x100;
                    res += (int)gotData[i];
                }
                for(int i=0; i<dicimal; i++)
                {
                    res/=10.0;
                }
                stringStream << res;
                retStr += typeName + ": " + stringStream.str() + ", ";
                //cout<<typeName+": "<<res<<endl;
            }
            else if(dataDefine.compare("string") == 0)
            {
                string res = "";
                for(int i=start; i < start+len; i++)
                {
                    res+=(char)gotData[i];
                }
                retStr += "\""+typeName + "\": \"" + res + "\", ";
                //cout<<typeName+": "<<res<<endl;   
            }
        }

        char detail[100]={0};
        sprintf(detail,"add custom test success");
        printlog(INFO, "control", detail);

        printf("[%s] ctrl_success\n",operation.c_str());
        return "[resault] " + retStr;
    }
    else if(operation.compare("Addcustomconfirm") == 0)
    {
        printf("Addcustomconfirm\n");
        int res, addr = atoi(root["addr"].asString().c_str());
        int returnLength = atoi(root["returnLength"].asString().c_str());
        string code_485 = root["code485"].asString();

        if(addr < 10)
        {
            printf("[%s] wrong addr\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"addr %d can not be changed",addr);
            printlog(INFO, "control", detail);
            return "this addr can not be changed!" ;
        }

        char sql[100]={0};
        //查看此addr状态 如果active则不
        /*
        res = IsActiveSensor(addr);
        if(res == ActiveSensor)
        {
            char detail[100]={0};
            sprintf(detail,"add custom failed, a sensor is running on addr %d",addr);
            printlog(INFO, "control", detail);
            return "a sensor is running on this addr";    
        }
        else
        {
            //删除此addr的sensor表和types表
            sprintf(sql,"delete from sensors where addr=%d;",addr);
            res = sendSQL(sql);
            sprintf(sql,"delete from types where addr=%d;",addr);
            res = sendSQL(sql);
        }*/
        
        //删除此addr的sensor表和types表
        sprintf(sql,"delete from sensors where addr=%d;",addr);
        res = sendSQL(sql);
        sprintf(sql,"delete from types where addr=%d;",addr);
        res = sendSQL(sql);

        //插入sensor表
        sprintf(sql,"insert into sensors values(%d,\"%s\",%d,1);",addr,code_485.c_str(),returnLength);
        res = sendSQL(sql);
        if(res != 0)
        {
            printf("[%s] insert sensor error\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"[%s] sql error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "failed, sql error";
        }

        for(unsigned int i=0; i<root["types"].size(); i++)
        {
            string typeName = root["types"][i]["typeName"].asString();    
            int start = atoi(root["types"][i]["byteStart"].asString().c_str());
            int len = atoi(root["types"][i]["byteCount"].asString().c_str());
            string dataDefine = root["types"][i]["dataDefine"].asString();
            int dicimal = atoi(root["types"][i]["dicimal"].asString().c_str());
            int negative = atoi(root["types"][i]["negative"].asString().c_str());

            sprintf(sql,"insert into types values(%d,\"%s\",%d,%d,\"%s\",%d,%d);",addr,typeName.c_str(),start,len,dataDefine.c_str(),dicimal,negative);
            res = sendSQL(sql);
            if(res != 0)
            {
                printf("[%s] insert sensor error\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sql error(in for)",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "failed, sql error";
            }
        }

        char detail[100]={0};
        sprintf(detail,"add sensor %d",addr);
        printlog(INFO, "control",detail);
        return "success";
    }
    else if(operation.compare("Delete") == 0)
    {
        int addr = atoi(root["content"].asString().c_str());
        int ret = IsActiveSensor(addr);
        if(ret == ActiveSensor)
        {
            int res = setSensorState(addr,0);
            if(res == 0)
            {
                char detail[100]={0};
                sprintf(detail,"sensor %d turn off",addr);
                printlog(INFO, "control",detail);
                
                if(addr >10 && addr <254)
                {
                    //删除此addr的sensor表和types表
                    char sql[100]={0};
                    sprintf(sql,"delete from sensors where addr=%d;",addr);
                    res = sendSQL(sql);
                    sprintf(sql,"delete from types where addr=%d;",addr);
                    res = sendSQL(sql);
                }
                return "success";   
            }
            else
            {
                char detail[100]={0};
                sprintf(detail,"failed to turn off the sensor");
                printlog(INFO, "control", detail);
                return "fail to turn off the sensor, change state failed";
            }
        }
        else if(ret == NotActiveSensor)
        {
            char detail[100]={0};
            sprintf(detail,"fail to turn off the sensor, it's not running");
            printlog(INFO, "control", detail);
            return "this sensor is not running";    
        }
        else if(ret == NullAddr)
        {
            char detail[100]={0};
            sprintf(detail,"fail to turn off the sensor, addr is not using");
            printlog(INFO, "control", detail);
            return "this addr hasn't been used";      
        }
        else
        {
            char detail[100]={0};
            sprintf(detail,"fail to turn off the sensor, unknown error");
            printlog(INFO, "control", detail);
            return "unknown error";    
        }
        
    }
    else if(operation.compare("Debug") == 0)
    {
        printf("Debug\n");

        string code_485 = root["content"].asString();
        string tmpstr;
        unsigned char temp[30] = {0};
        int len=0, ret;
        unsigned char gotData[200]={0};
        string retstr = "";

        for(int i=0; i<code_485.size(); i+=3)
        {
            tmpstr = code_485.substr(i,2);
            int a = (int)strtol((char*)tmpstr.c_str(),NULL,16);
            temp[len++] = (unsigned char)a;
        }

        //debug 专用接口
        ret = DebugSendCode(com_fd,(char*)temp,len,(char*)gotData);

        if(ret == 0)
        {
            printf("[%s] sensor time out! check the sensor!\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"[%s] sensor time out",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "sensor time out! check the sensor or your code!";
        }
        else if(ret < 0)
        {    
            printf("[%s] sensor select error!check the sensor!\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"[%s] sensor select error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "sensor select error!check the sensor!";
        }

        for(int i=0;i<ret;i++)
        {
            char tempchar[10]={0};
            sprintf(tempchar,"%02x",gotData[i]);
            string tmpstring = tempchar;

            retstr += tmpstring;
            if(i != ret-1)
            {
                retstr += " ";
            }
        }

        char detail[100]={0};
        sprintf(detail,"debug code:%s, return code:%s",code_485.c_str(),retstr.c_str());
        printlog(INFO, "control",detail);
        
        return retstr;
    }
    else
    {
        return "payload_error";
    }
}

string relay_opt(string operation, string contentstr, string addrstr)
{
    unsigned short content;
    unsigned char addr;
    if(contentstr.compare("") != 0)
    {
        std::stringstream stringStream;
        stringStream << contentstr;
        stringStream >> content;
    }
    addr = atoi(addrstr.c_str());

    if(operation.compare("On1") == 0 || operation.compare("Off1") == 0)
    {
        //打开或关闭继电器的一个引脚
        printf("-------relay on/off 1---------\n");
        //cout << root["content"].asString() << endl;
        unsigned char temp[20]={0xfe,0x05,0x00,0x00,0x00,0x00};
        char gotData[50];
        int len = 6, needlen = 8, ret;

        if(operation.compare("On1") == 0)
        {
            temp[4] = 0xff;
        }

        if(content < 0 || content > 7)
        {
            printf("[%s]this data is not available\n",operation.c_str());
            char detail[100]={0};
            sprintf(detail,"[%s] data is unavailable",__FUNCTION__);
            printlog(INFO, "control", detail);
            return "this data is not available!";
        }

        temp[0] = addr;

        //temp[2] = (content-1) >> 8;
        temp[2] = content >> 8;
        //temp[3] = (content-1) & 0x00ff;
        temp[3] = content & 0x00ff;

        CRC_Calculater(temp, len);

        len+=2;

        ret = SendCode(com_fd,(char*)temp,len,needlen,gotData);
        if(SensorGetDataOK != ret)
        {
            if(ret == SensorTimeOut)
            {
                printf("[%s] sensor time out! check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor time out",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor time out! check the sensor!";
            }
            else if(ret == SensorSelectFailed)
            {    
                printf("[%s] sensor select error!check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor select error",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor select error!check the sensor!";
            }
            else if(ret == SensorGotWrongData)
            {
                printf("[%s] got wrong data, maybe sensor is unavailable\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] got wrong data",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "got wrong data, maybe sensor is unavailable";
            }
        }

        char detail[100]={0};
        sprintf(detail,"relay %s %d",operation.c_str(),(int)content+1);
        printlog(INFO, "control",detail);
        printf("[%s] ctrl_success\n",operation.c_str());
        return "success";
    }
    else if(operation.compare("OnAll") == 0 || operation.compare("OffAll") == 0)
    {
        //打开或关闭继电器的所有引脚
        printf("-------relay on/off all---------\n");
        //cout << root["content"].asString() << endl;
        unsigned char temp[20]={0xfe,0x0f,0x00,0x00,0x00,0x08,0x01,0x00};
        char gotData[50];
        int len = 8, needlen = 8, ret;

        if(operation.compare("OnAll") == 0)
        {
            temp[7] = 0xff;
        }

        temp[0] = addr;

        CRC_Calculater(temp, len);

        len+=2;

        ret = SendCode(com_fd,(char*)temp,len,needlen,gotData);
        if(SensorGetDataOK != ret)
        {
            if(ret == SensorTimeOut)
            {
                printf("[%s] sensor time out! check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor time out",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor time out! check the sensor!";
            }
            else if(ret == SensorSelectFailed)
            {    
                printf("[%s] sensor select error!check the sensor!\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor select error",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "sensor select error!check the sensor!";
            }
            else if(ret == SensorGotWrongData)
            {
                printf("[%s] got wrong data, maybe sensor is unavailable\n",operation.c_str());
                char detail[100]={0};
                sprintf(detail,"[%s] sensor got wrong data",__FUNCTION__);
                printlog(WARN, "control", detail);
                return "got wrong data, maybe sensor is unavailable";
            }
        }

        char detail[100]={0};
        sprintf(detail,"relay %s",operation.c_str());
        printlog(INFO, "control",detail);
        printf("[%s] ctrl_success\n",operation.c_str());
        return "success";
    }
    else
    {
        return "payload_error";
    }
}

string rule_opt(Json::Value root)
{
    string operation;
    operation = root["operation"].asString();

    if(operation.compare("add") == 0)
    {
        string id = root["id"].asString();
        int addr = atoi(root["addr"].asString().c_str());
        string typeName = root["typeName"].asString();
        string logic = root["logic"].asString();
        string value = root["value"].asString();
        string code_485 = root["code_485"].asString();
        //int switchState = atoi(root["switchState"].asString().c_str());
        int switchState = 0;
        printf("*********** -%d-  *************\n",switchState);

        int res;
        char sql[200]={0};
        char *szErrMsg;

        sprintf(sql,"insert into rules values(\"%s\",%d,\"%s\",\"%s\",\"%s\",\"%s\",0,%d);",id.c_str(),addr,
        typeName.c_str(),
        logic.c_str(),
        value.c_str(),
        code_485.c_str(),
        switchState);
        //printf("[setRuleState] prepare to sql :%s\n",selectSql);
        res = sqlite3_exec(db,sql, NULL, NULL, &szErrMsg);
        if(res != SQLITE_OK)
        {
            printf("[%s] %s\n",__FUNCTION__,szErrMsg);
            sqlite3_free(szErrMsg);
            char detail[100]={0};
            sprintf(detail,"[%s] sql error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "failed, sql error";
        }
        sqlite3_free(szErrMsg);
        
        char detail[100]={0};
        sprintf(detail,"add rule %s",id.c_str());
        printlog(INFO, "control",detail);
        printf("[%s] add rule success\n",__FUNCTION__);
        return "success";
    }
    else if(operation.compare("delete") == 0)
    {
        string id = root["id"].asString();
        int res;
        char sql[200]={0};
        char *szErrMsg;

        sprintf(sql,"delete from rules where id=\"%s\";",id.c_str());
        //printf("[setRuleState] prepare to sql :%s\n",selectSql);
        res = sqlite3_exec(db,sql, NULL, NULL, &szErrMsg);
        if(res != SQLITE_OK)
        {
            printf("[%s] %s\n",__FUNCTION__,szErrMsg);
            sqlite3_free(szErrMsg);
            char detail[100]={0};
            sprintf(detail,"[%s] sql error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "failed, sql error";
        }
        sqlite3_free(szErrMsg);
        
        char detail[100]={0};
        sprintf(detail,"delete rule %s",id.c_str());
        printlog(INFO, "control",detail);
        printf("[%s] delete rule success\n",__FUNCTION__);
        return "success";

    }
    else if(operation.compare("setSwitchState") == 0)
    {
        string id = root["id"].asString();
        int switchState = atoi(root["switchState"].asString().c_str());
        int res;
        char sql[200]={0};
        char *szErrMsg;

        sprintf(sql,"update rules set switchState=%d,trigerState=0 where id=\"%s\";",switchState,id.c_str());
        //printf("[setRuleState] prepare to sql :%s\n",selectSql);
        res = sqlite3_exec(db,sql, NULL, NULL, &szErrMsg);
        if(res != SQLITE_OK)
        {
            printf("[%s] %s\n",__FUNCTION__,szErrMsg);
            sqlite3_free(szErrMsg);
            char detail[100]={0};
            sprintf(detail,"[%s] sql error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "failed, sql error";
        }
        sqlite3_free(szErrMsg);
        
        char detail[100]={0};
        sprintf(detail,"set rule(id:%s)'s switchState to %d",id.c_str(),switchState);
        printlog(INFO, "control",detail);
        printf("[%s] success\n",__FUNCTION__);
        return "success";
        
    }
    else if(operation.compare("changeRule") == 0)
    {
        string id = root["id"].asString();
        int addr = atoi(root["addr"].asString().c_str());
        string typeName = root["typeName"].asString();
        string logic = root["logic"].asString();
        string value = root["value"].asString();
        string code_485 = root["code_485"].asString();
        int switchState = atoi(root["switchState"].asString().c_str());

        int res;
        char sql[200]={0};
        char *szErrMsg;

        sprintf(sql,"update rules set addr=%d,typeName=\"%s\",logic=\"%s\",value=\"%s\",code_485=\"%s\",trigerState=0,switchState=0 where id=\"%s\";",addr,
        typeName.c_str(),
        logic.c_str(),
        value.c_str(),
        code_485.c_str()),
        id.c_str();
        //printf("[setRuleState] prepare to sql :%s\n",selectSql);
        res = sqlite3_exec(db,sql, NULL, NULL, &szErrMsg);
        if(res != SQLITE_OK)
        {
            printf("[%s] %s\n",__FUNCTION__,szErrMsg);
            sqlite3_free(szErrMsg);
            char detail[100]={0};
            sprintf(detail,"[%s] sql error",__FUNCTION__);
            printlog(WARN, "control", detail);
            return "failed, sql error";
        }
        sqlite3_free(szErrMsg);
        printf("[%s] change rule success\n",__FUNCTION__);
        char detail[100]={0};
        sprintf(detail,"change rule success");
        printlog(INFO, "control", detail);
        return "success";
    }
    else
    {
        printf("[%s] payload_error\n",__FUNCTION__) ;
        return "payload_error";   
    }
}

string choose_handle(string json_payload)
{
    string target = "";
    string operation = "";
    string content = "";

    Json::Reader reader;
    Json::Value root;

    reader.parse(json_payload,root);

    target = root["target"].asString();
    operation = root["operation"].asString();
    content = root["content"].asString();

    //cout << "target" << typeid(target).name() << endl;
    //cout << "operation" << typeid(operation).name() << endl;
    //cout << "content" << typeid(content).name() << endl;

    if( target.compare("relay") == 0 )
    {
        //继电器操作 特殊处理
        return relay_opt(operation,content,root["addr"].asString());
    }
    else if( target.compare("sensor") == 0 )
    {
        //传感器操作 特殊处理
        return sensor_opt(root);
    }
    else if( target.compare("DOA") == 0 )
    {
        return doa_opt(operation, content);
    }
    else if( target.compare("sendrate") == 0 )
    {
        return sendrate_opt(operation, content);
    }
    else if( target.compare("rule") == 0 )
    {
        //规则操作  特殊处理
        return rule_opt(root);
    }
    else if( target.compare("heart") == 0)
    {
        return "heart";    
    }
    else
    {
        printf("payload error!\n");
        printlog(INFO, "control","payload error");
        return "payload error";
    }
}

int pubMessage(char pub_topic[],char payload[])
{
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    char EncodePayload[2048] = {0};
    int ret = 0;
    //加密   return: 成功时返回结果字符串长度; -1 arg error
    ret = BASE64_Encode( (const char*)payload, strlen(payload), EncodePayload);
    if(ret < 0)
    {
        printf("[pubMessage] base64 encode error\n") ;   
    }

    pubmsg.payload = (void *)EncodePayload;
    pubmsg.payloadlen = strlen(EncodePayload);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;

    MQTTClient_publishMessage(client, pub_topic, &pubmsg, &token);

    int rc;
    printf("Waiting for pub message on topic -%s- with ClientID: %s\n",
            pub_topic, MAC);
    rc = MQTTClient_waitForCompletion(client, token, MQTTTIMEOUT);
    printf("Message pub success! token is %d \n", token);

    return rc;
}


void *choose_pubtopic(void *payload)
{
    char json_payload[1024]={0};
    //解密   return: 成功时返回结果字符串长度; -1 arg error; -2 数据错误 非BASE64
    int res = BASE64_Decode( (const unsigned char*)payload, strlen((char*)payload), (char*)json_payload);
    if(res < 0)
    {
        printf("[Recv Message] decode failed\n");
        return NULL;
    }

    if(IsJsonIllegal(json_payload) == false)
    {
        printf("PayLoad ERROR!\n");
        return NULL;
    }

    printf("Message: ");
    for(int i=0;i<strlen(json_payload);i++)
    {
        printf("%c",json_payload[i]);   
    }
    printf("\n");

    string jsonpayload = "";
    string pubtopic = "";

    jsonpayload = (char*)json_payload;
    Json::Reader reader;
    Json::Value root;

    reader.parse(jsonpayload,root);
    pubtopic  = root["topic"].asString();

    //cout << pubtopic << endl;

    char pub_topic[100] = {0};
    strcpy(pub_topic,pubtopic.c_str());

    string ret;
    ret = choose_handle(jsonpayload);			//ret：0 操作成功  1 操作失败 -1 payload格式错误

    pubMessage(pub_topic, (char*)ret.c_str());
}


void *subMessage(void *sub_topic)
{
    MQTTClient_subscribe(client, (char*)sub_topic, 1);     /* 订阅一个客户端 的一个话题*/
    MQTTClient_message *receivemsg = NULL ;	
    char* payloadptr;

    int   i,rc;	
    char* topicName_rev = NULL;
    int   topicLen_rev;

    usleep(10000);
    while(1)
    {
        if(MQTTClient_isConnected(client) == true)   /* 检测连接状态 */
        {
            //printf("alive \n");
        }
        else
        {
            printf(" heart beat failed \n");
            //log
            break;
        }
        rc = MQTTClient_receive(client,&topicName_rev, &topicLen_rev, &receivemsg,5000); /* 接收消息 */
        if(rc == MQTTCLIENT_SUCCESS)
        {
            //printf("topicName: %s  topicName_LEN: %d \n", topicName_rev,topicLen_rev);
            if(topicName_rev != NULL)                          /* 滤掉心跳包 */
            {
                printf("*********Received Message********\n");
                //log
                printf("topicName: ");
                for(i=0;i<topicLen_rev;i++)
                {
                    printf("%c", topicName_rev[i]);
                }
                printf("\n");
                payloadptr = (char *)receivemsg->payload;

                *(payloadptr + receivemsg->payloadlen) = 0;
                string json_payload = "";
                json_payload = payloadptr;

                printf("payload_len:%d msgid: %d \n",receivemsg->payloadlen,receivemsg->msgid);

                pthread_t choose_pubtopic_thid;
                if(pthread_create(&choose_pubtopic_thid,NULL,choose_pubtopic,(void *)payloadptr) != 0)
                {
                    printf("choose_pubtopic thread creation failed!\n");
                }
            }
        }

        usleep(10000);
        usleep(100000);
    }
}


