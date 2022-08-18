#include "include.h"
using namespace std;

int getConfig()
{
    int fd,ret;
    char configstr[1024];
    Json::Reader reader;

    fd = open("./config.json",O_RDONLY);
    
    ret = read(fd,configstr,1000);
    if(ret <=0 || ret >=900)
    {
        return -1;
    }
    close(fd);
    reader.parse(configstr,configJson);
    return 0;
}

int setConfig()
{
    int fd,ret;
    string configstr;
    configstr = configJson.toStyledString();
    
    fd = open("./config.json",O_WRONLY);

    ret = write(fd,configstr.c_str(),strlen(configstr.c_str()));
    if(ret <=0 || ret >=900)
    {
        return -1;
    }
    close(fd);
    
    return 0;
}
