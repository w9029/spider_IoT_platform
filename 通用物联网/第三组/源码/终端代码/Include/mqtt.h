#ifndef __mqtt__
#define __mqtt__
#include "include.h"

using namespace std;

int IsActiveSensor(int addr);
int setSensorState(int addr, int state);
int sendSQL(char *sql);

bool IsJsonIllegal(const char *jsoncontent);

string sendrate_opt(string operation, string content);
string doa_opt(string operation, string content);
string sensor_opt(Json::Value root);
string relay_opt(string operation, string content, string addrstr);
string rule_opt(Json::Value root);

string choose_handle(string json_payload);

//void sig_handler(int signo);

int pubMessage(char pub_topic[],char payload[]);

void *choose_pubtopic(void *json_payload);

void *subMessage(void *sub_topic);

#endif


