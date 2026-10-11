#include "sdk/mi.h"

/* Hand-written ARM assembly in the SDK: a loop that tests first and stores under a condition. */
asm void MIi_CpuClear32(u32 data, void *dest, int size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt loop
    bx lr
}
