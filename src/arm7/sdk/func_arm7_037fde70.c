#include "sdk/mi.h"

asm void func_arm7_037fde70(const void *src, void *dest, int size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt loop
    bx lr
}
