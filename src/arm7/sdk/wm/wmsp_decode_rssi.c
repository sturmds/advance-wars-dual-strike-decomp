#include "types.h"

int Wmsp_DecodeRssi(int rawRssi)
{
    if (rawRssi & 2) {
        return (rawRssi >> 2) & 0xff;
    }
    return ((rawRssi >> 2) + 25) & 0xff;
}
