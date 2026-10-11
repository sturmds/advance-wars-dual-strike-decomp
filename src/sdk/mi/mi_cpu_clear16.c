#include "sdk/mi.h"

/* Hand-written ARM assembly in the SDK: a loop that tests first and stores under a condition. */
asm void MIi_CpuClear16(u16 data, void *dest, int size)
{
    mov r3, #0
loop:
    cmp r3, r2
    strlth r0, [r1, r3]
    addlt r3, r3, #2
    blt loop
    bx lr
}
