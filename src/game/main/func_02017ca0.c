#include "types.h"

extern int gPlayRecords;

void func_02017ca0(int a0)
{
    if ((u32)gPlayRecords >= (u32)a0) {
        gPlayRecords = a0;
    }
}
