#include "include.h"
#include "printlog.h"
#include "linkDB.h"

using namespace std;

int callback(void *NotUsed,int argc, char **argv, char **azColName)
{
	int i;
	for(i = 0;i<argc;i++)
	{
		printf("%s = %s\n",azColName[i],argv[i]?argv[i]:"NULL");
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
		printlog(INFO, "DataBase", "database open error");
		exit(0);
	}
	else
	{
		//printf("open database successfully\n");
		printlog(INFO, "DataBase", "database open successfully");
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

