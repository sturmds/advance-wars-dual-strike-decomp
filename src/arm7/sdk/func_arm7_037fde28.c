#include "sdk/mi.h"

asm void func_arm7_037fde28(u16 data, void *dest, int size)
{
    mov r3, #0
loop:
    cmp r3, r2
    strlth r0, [r1, r3]
    addlt r3, r3, #2
    blt loop
    bx lr
}
