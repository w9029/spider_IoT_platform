#ifndef __printlog__
#define __printlog__
#define DEBUG 0
#define INFO 1
#define WARN 2
#define ERROR 3

void printlog(int level, const char type[], const char detail[]);

#endif

