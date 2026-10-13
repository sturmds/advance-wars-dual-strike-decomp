#include "types.h"

#ifdef __MWERKS__
/* Sets bit 0 of the CP15 control register: turns the memory protection unit on. */
asm void OS_EnableProtectionUnit(void)
{
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}
#else
#error "OS_EnableProtectionUnit: setting the protection unit bit in the CP15 control register (mrc/mcr) has no portable C equivalent"
#endif
