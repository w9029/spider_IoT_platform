#include "include.h"
#include "CRC.h"

using namespace std;

void CRC_Calculater(unsigned char bytes[], int len)
{
    unsigned short CRC = 0xffff;
    int POLYNOMIAL = 0xa001;

    int i, j;
    for (i = 0; i < len; i++) {
        CRC ^= (int) bytes[i];
        for (j = 0; j < 8; j++) {
            if ((CRC & 0x0001) == 1) {
                CRC >>= 1;
                CRC ^= POLYNOMIAL;
            } else {
                CRC >>= 1;
            }
        }
    }

    CRC = (CRC & 0x00ff) << 8 | (CRC & 0xff00)>>8;

    bytes[len] = CRC>>8;
    bytes[len+1] = CRC & 0x00ff;
}
