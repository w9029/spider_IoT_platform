#ifndef __sensorstruct__
#define __sensorstruct__
//#include "include.h"
#include<string>
using namespace std;

typedef struct SENSOR{
	int addr;
	string code_485;
	int returnLength;
	int state;
}sensor;

typedef struct SENSOR_TYPE{
	int addr;
	string typeName;
	int byteStart;
	int byteCount;
	string dataDefine;
	int dicimal;
    int negative;
}sensor_type;

#endif

