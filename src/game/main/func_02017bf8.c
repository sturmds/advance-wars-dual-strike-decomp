#include "types.h"

extern int gPlayRecords;

int func_02017bf8(int a0)
{
    int t0 = gPlayRecords;
    if ((u32)t0 < (u32)a0) {
        return 0;
    }
    gPlayRecords = t0 - a0;
    return 1;
}
