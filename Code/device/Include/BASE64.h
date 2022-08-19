#ifndef __BASE64__
#define __BASE64__

//加密   return: 成功时返回结果字符串长度; -1 arg error
int BASE64_Encode( const char* inputBuffer, int inputCount, char* outputBuffer );

//解密   return: 成功时返回结果字符串长度; -1 arg error; -2 数据错误 非BASE64
int BASE64_Decode( const unsigned char* inputBuffer, int inputCount, char* outputBuffer );

#endif
