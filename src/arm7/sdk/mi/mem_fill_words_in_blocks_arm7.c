#include "sdk/mi.h"

asm void Mem_FillWordsInBlocks_Arm7(u32 data, void *dest, u32 size)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
block:
    cmp r1, r12
    stmltia r1!, {r0, r2, r3, r4, r5, r6, r7, r8}
    blt block
word:
    cmp r1, r9
    stmltia r1!, {r0}
    blt word
    ldmia sp!, {r4, r5, r6, r7, r8, r9}
    bx lr
}
