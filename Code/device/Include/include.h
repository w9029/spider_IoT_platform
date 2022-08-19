#ifndef __include__
#define __include__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <net/if.h>
#include <semaphore.h>
#include <sqlite3.h>
#include <iostream>
#include <signal.h>
#include "MQTTClient.h"
#include "json/json.h"

#include <string>
#include <vector>
#include <sstream>

#include "cstdlib"
#include <typeinfo>

//public
#include "printlog.h"

#define ADDRESS		"tcp://192.168.100.56:1883"
#define QOS			1
#define MQTTTIMEOUT 10000L

//sendCode
#define SensorGetDataOK 0
#define SensorTimeOut -1
#define SensorSelectFailed -2
#define SensorGotWrongData -3

//IsActiveSensor
#define ActiveSensor 1
#define NotActiveSensor 0
#define NullAddr -1
#define SQLERROR -2

//COM lock time
#define COMLOCKTIME 500000

//sendrate
#define SendRateMax 7200
#define SendRateMin 15

extern int com_fd;
extern MQTTClient client;
extern MQTTClient_connectOptions conn_opts;
extern sqlite3 *db;
extern FILE *logfp;
extern char *zErrMsg;
extern pthread_mutex_t sendratemutex;
extern pthread_mutex_t configmutex;
extern pthread_mutex_t commutex;
extern pthread_mutex_t logmutex;
extern char MAC[];
extern int GetDataRate;
extern Json::Value configJson;

#endif
