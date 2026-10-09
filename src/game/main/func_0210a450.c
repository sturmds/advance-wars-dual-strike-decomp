#include "types.h"

int func_0210a450(int a0)
{
    int t0 = *((int *)0x4001000);
    int t1 = t0 & -8;
    int t2 = t1 | a0;
    *((int *)0x4001000) = t2;
    return t2;
}
