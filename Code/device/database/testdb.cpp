#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
#include<time.h>
#include<json/json.h>
#include<sqlite3.h>

using namespace std;

int fd, fdw, sfd, videofd, firstflag=1, connectflag;
FILE *logfp;
sqlite3 *db;

char *zErrMsg = 0;

void printlog(int level, time_t time, const char type[], const char detail[])
{
	char s[100], temp[100], *c;
	logfp = fopen("./log", "a");

	strcpy(s, ctime(&time));
	s[strlen(s)-1] = '\0';
	fprintf(logfp, "[LEVEL%d] [%s] %s: %s\n", level, s, type, detail);

	fclose(logfp);
}


static int callback(void *NotUsed,int argc, char **argv, char **azColName)
{
	int i;
	for(i = 0;i<argc;i++)
	{
		printf("----%d----\n:%s = %s\n",i,azColName[i],argv[i]?argv[i]:"NULL");
	}
	printf("\n");
	return 0;
}


void linkDB(char DBfilename[])
{
	int ret;

	ret = sqlite3_open(DBfilename, &db);

	if(ret)
	{
		printf("open error, %s\n",sqlite3_errmsg(db));
		printlog(3, time(NULL), "DataBase", "database open error");
		exit(0);
	}
	else
	{
		printf("open database successfully\n");
		printlog(3, time(NULL), "DataBase", "database open successfully");
	}
	//create sql statement
    /*
	char sql[] = "CREATE TABLE Data(\n"\
				  "time INTEGER PRIMARY KEY NOT NULL,\n"\
				  "temperature REAL NOT NULL,\n"\
				  "humidity REAL NOT NULL,\n"\
				  "ammonia REAL NOT NULL);";

	//execute sql statement
	ret = sqlite3_exec(db,sql,callback,0,&zErrMsg);
	if(ret != SQLITE_OK)
	{
		printf("SQL:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		printf("table create success\n");
	}*/
}

int main(int argc,char *argv[])
{
	if(argc < 2)
	{	
		printf("need an arg\n");
		exit(0);
	}
	printf("%s\n",argv[1]);
	linkDB(argv[1]);
	
    int ret;
    char sql []="update sensors set state = 1 where addr =1;";
	ret = sqlite3_exec(db,sql,callback,0,&zErrMsg);
    printf("set ok\n");

	return 0;
}
