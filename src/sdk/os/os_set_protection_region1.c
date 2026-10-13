#include "types.h"

#ifdef __MWERKS__
/* Writes the CP15 protection region 1 register (base, size and enable bit). */
asm void OS_SetProtectionRegion1(u32 region)
{
    mcr p15, 0, r0, c6, c1, 0
    bx lr
}
#else
#error "OS_SetProtectionRegion1: writing CP15 protection region 1 (mcr) has no portable C equivalent"
#endif
