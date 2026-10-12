#include "sdk/mi.h"

asm void Mem_CopyHalfwords_Arm7(const void *src, void *dest, u32 size)
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
