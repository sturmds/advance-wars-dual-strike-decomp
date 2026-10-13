#include "types.h"

#ifdef __MWERKS__
/* Discards the whole instruction cache. */
asm void IC_InvalidateAll(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c5, 0
    bx lr
}
#else
#error "IC_InvalidateAll: invalidating the instruction cache (mcr) has no portable C equivalent"
#endif
