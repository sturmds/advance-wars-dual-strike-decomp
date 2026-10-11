#include "sdk/mi.h"

/* Hand-written ARM assembly in the SDK: a loop that tests first and copies under a condition. */
asm void MIi_CpuCopy32(const void *src, void *dest, int size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt loop
    bx lr
}
