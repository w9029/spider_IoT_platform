#include<stdio.h>
#include<sqlite3.h>
#include<string.h>
int main()
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    //1
    int res = sqlite3_open("device.db",&db);
    if(res != SQLITE_OK)
    {
        fprintf(stderr,"open error %s\n",sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }
    printf("打开数据库test.db成功～\n");

    //2.
    char *selectSql = "select * from types;";
    res = sqlite3_prepare_v2(db,selectSql,strlen(selectSql),&stmt,0);
    if(res != SQLITE_OK)
    {
        printf("sqlite3_prepare_v2 error\n");
        fprintf(stderr,"prepare error %s\n",sqlite3_errmsg(db));
        return -1;
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
    printf("count = %d\n",count);
    int i;
    //获取字段名和字段类型
    for(i = 0; i < count; i++)
    {
        printf("column name:%s\n",sqlite3_column_name(stmt,i));
        printf("column type:%i\n",sqlite3_column_type(stmt,i));
    }

    //获取字段值
    int addr;
    const unsigned char *typeName;
    int byteStart;
    int byteCount;
    const unsigned char *dataDefine;
    int dicimal;
    while(res == SQLITE_ROW)
    {
        addr = sqlite3_column_int(stmt,0);
        typeName = sqlite3_column_text(stmt,1);
        byteStart = sqlite3_column_int(stmt,2);
        byteCount = sqlite3_column_int(stmt,3);
        dataDefine = sqlite3_column_text(stmt,4);
        dicimal = sqlite3_column_int(stmt,5);
        printf("addr = %d,typeName = %s,byteStart = %d,byteCount = %d,dataDefine = %s,dicimal = %d\n",
        addr,typeName,byteStart,byteCount,dataDefine,dicimal);

        res = sqlite3_step(stmt);
    }
    //4
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
