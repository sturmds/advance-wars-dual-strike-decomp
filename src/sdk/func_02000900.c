#include "sdk/mi.h"

asm void func_02000900(u32 data, void *dest, int size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt loop
    bx lr
}
