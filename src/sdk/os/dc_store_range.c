#include "types.h"

#ifdef __MWERKS__
/* Writes the data cache lines (32 bytes each) that cover [start, start + size) back to memory. */
asm void DC_StoreRange(void *start, u32 size)
{
    add r1, r1, r0
    bic r0, r0, #0x1f
loop:
    mcr p15, 0, r0, c7, c10, 1
    add r0, r0, #0x20
    cmp r0, r1
    blt loop
    bx lr
}
#else
#error "DC_StoreRange: cleaning data cache lines (mcr) has no portable C equivalent"
#endif
