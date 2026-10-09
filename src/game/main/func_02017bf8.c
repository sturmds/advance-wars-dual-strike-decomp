#include "types.h"

extern int data_02290718;

int func_02017bf8(int a0)
{
    int t0 = data_02290718;
    if ((u32)t0 < (u32)a0) {
        return 0;
    }
    data_02290718 = t0 - a0;
    return 1;
}
