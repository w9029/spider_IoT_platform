#include "include.h"
#include "printlog.h"

void printlog(int level, const char type[], const char detail[])
{
	char s[100], temp[100], *c;
    char levelstr[20]={0};
    time_t t;
	
    pthread_mutex_lock(&logmutex);

    logfp = fopen("./log", "a");

    t = time(NULL);
	strcpy(s, ctime(&t));
	s[strlen(s)-1] = '\0';

    switch(level)
    {
        case DEBUG:{
                strcpy(levelstr,"DEBUG");
                break;
            }
        case INFO:{
                strcpy(levelstr,"INFO");
                break;
            }
        case WARN:{
                strcpy(levelstr,"WARN");
                break;
            }
        case ERROR:{
                strcpy(levelstr,"ERROR");
                break;
            }
        default:
            return;
    }
	fprintf(logfp, "[%s] [%s] %s: %s\n", levelstr, s, type, detail);

	fclose(logfp);
    pthread_mutex_unlock(&logmutex);
}


