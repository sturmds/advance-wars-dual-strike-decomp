#include "sdk/mi.h"

asm void func_arm7_037fde40(const void *src, void *dest, int size)
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
