#include "sdk/mi.h"

/* Hand-written ARM assembly in the SDK: a loop that tests first and copies under a condition. */
asm void MIi_CpuCopy16(const void *src, void *dest, int size)
{
    mov r12, #0
loop:
    cmp r12, r2
    ldrlth r3, [r0, r12]
    strlth r3, [r1, r12]
    addlt r12, r12, #2
    blt loop
    bx lr
}
