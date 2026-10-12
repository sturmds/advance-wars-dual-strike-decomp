#include "sdk/mi.h"

asm void Mem_FillWords_Arm7(u32 data, void *dest, u32 size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt loop
    bx lr
}
