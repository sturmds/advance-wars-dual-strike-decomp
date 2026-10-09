#include "types.h"

void func_020242c0(int *a0)
{
    int t0 = *a0 + 1;
    if ((u32)t0 >= (u32)-0x7f41043c) {
        *a0 = -0x7f41043c;
    } else {
        *a0 = t0;
    }
}
