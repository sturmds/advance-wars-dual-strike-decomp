#include "types.h"

/* Discards the instruction cache lines (32 bytes each) that cover [start, start + size). */
asm void IC_InvalidateRange(void *start, u32 size)
{
    add r1, r1, r0
    bic r0, r0, #0x1f
loop:
    mcr p15, 0, r0, c7, c5, 1
    add r0, r0, #0x20
    cmp r0, r1
    blt loop
    bx lr
}
