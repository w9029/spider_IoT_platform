#include "include.h"
#include "RuleJudger.h"
#include "portOpt.h"

using namespace std;

int setRuleTrigerState(string id, int trigerState)
{
    int res;
    char selectSql[100]={0};
    char *szErrMsg;

    sprintf(selectSql,"update rules set trigerState=%d where id=\"%s\";",trigerState,id.c_str());
    //printf("[setRuleState] prepare to sql :%s\n",selectSql);
    res = sqlite3_exec(db,selectSql, NULL, NULL, &szErrMsg);
    if(res != SQLITE_OK)
    {
        printf("[setRuleState] %s\n",szErrMsg);
        sqlite3_free(szErrMsg);
        return -1;
    }
    sqlite3_free(szErrMsg);

    return 0;
}

int RuleJudger(int addr, string typeName, string strValue)
{
    sqlite3_stmt *stmt;
    int res;
    char select[100];
    double value = atof(strValue.c_str());

    //用于返回 如果有规则判断正确则1 无则0
    int returnFlag;

    sprintf(select, "select id,logic,value,code_485,trigerState from rules where switchState=1 and addr=%d and typeName=\"%s\";",addr,typeName.c_str());
    res = sqlite3_prepare_v2(db,select,strlen(select),&stmt,0);
    if(res != SQLITE_OK)
    {
        printf("\n[RuleJudger] sqlite3_prepare_v2 error\n");
        //fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));

        return 0;
    }
    //3
    res = sqlite3_step(stmt);

    int rulesCount = sqlite3_column_count(stmt);

    string id;
    string logic;
    string ruleValueStr;
    string code_485;
    //int returnLength;
    int trigerState;

    //循环获取规则 并判断
    while(res == SQLITE_ROW)
    {
        id = (char*)sqlite3_column_text(stmt,0);
        logic = (char*)sqlite3_column_text(stmt,1);
        ruleValueStr = (char*)sqlite3_column_text(stmt,2);
        code_485 = (char*)sqlite3_column_text(stmt,3);
        trigerState = sqlite3_column_int(stmt,4);

        double ruleValue = atof(ruleValueStr.c_str());

        //存放对比结果
        int resault= 0;

        //判断规则
        if(logic.compare("moreThan") == 0)
        {
            resault = value > ruleValue? 1:0;
        }
        else if(logic.compare("lessThan") == 0)
        {
            resault = value < ruleValue? 1:0;
        }
        else if(logic.compare("notMoreThan") == 0)
        {
            resault = value <= ruleValue? 1:0;
        }
        else if(logic.compare("notLessThan") == 0)
        {
            resault = value >= ruleValue? 1:0;
        }

        if(resault == 1)
        {
            returnFlag = 1;
        }

        if(resault == 1 && trigerState == 0)
        {
            printf("\n[RunleJudger] rule %s is trigered\n",id.c_str());
            //执行code 并且state置一
            char sendData[50]={0};
            char gotData[100]={0};
            int ret, len;
            len = codeTransfer(code_485,sendData);
            ret = DebugSendCode(com_fd, sendData, len, gotData);
            if(ret > 0)
            {
                //success   log  pubmessage
                //printf("[RuleJudger] rule %d code success\n",id);
                //state 置1
                if(setRuleTrigerState(id,1) == -1)
                {
                    //setState failed log pub
                }
                else
                {
                    //setRuleState success   
                    //printf("[RuleJudger] set rule %d state to %d\n",id,1);
                }
            }
            else
            {
                //failed   log  pubmsg

            }
        }
        else if(resault == 0 && trigerState == 1)
        {
            printf("\n[RunleJudger] rule %s is untrigered\n",id.c_str());
            //不成立 但是state为1 需要置零 （更新库）
            if(setRuleTrigerState(id,0) == -1)
            {
                //setState failed log pub
            }
            else
            {
                //setRuleState success   
                //printf("[RuleJudger] set rule %d state to %d\n",id,0);
            }

        }
        else
        {
            //无需操作

        }
        
        res = sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
    
    return returnFlag;
}
