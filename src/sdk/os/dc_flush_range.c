#include "types.h"

#ifdef __MWERKS__
/* Writes back and discards the data cache lines (32 bytes each) that cover [start, start + size). */
asm void DC_FlushRange(const void *start, u32 size)
{
    add r1, r1, r0
    bic r0, r0, #0x1f
loop:
    mcr p15, 0, r0, c7, c14, 1
    add r0, r0, #0x20
    cmp r0, r1
    blt loop
    bx lr
}
#else
#error "DC_FlushRange: cleaning and invalidating data cache lines (mcr) has no portable C equivalent"
#endif
