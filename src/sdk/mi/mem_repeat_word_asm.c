#include "types.h"

/*
 * Hand-written ARM assembly: fills size bytes at dest with a 32-bit value.
 * The conditional stmia is not something the compiler emits from C.
 */
asm void Mem_RepeatWordAsm(u32 value, u32 *dest, u32 size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt loop
    bx lr
}
