#include "types.h"

#ifdef __MWERKS__
/*
 * Sets the memory protection unit enable bit (bit 0 of the CP15 control register).
 * Returns the old bit. The second pair of instructions is an unused entry that clears it.
 */
asm BOOL Isd_EnableProtectionUnit(void)
{
    mov r0, #1
    b @set
    mov r0, #0
    b @set
@set:
    mrc p15, 0, r1, c1, c0, 0
    and r2, r1, #1
    tst r0, r0
    biceq r1, r1, #1
    orrne r1, r1, #1
    mcr p15, 0, r1, c1, c0, 0
    mov r0, r2
    bx lr
}
#else
#error "Isd_EnableProtectionUnit: setting the protection unit bit in the CP15 control register (mrc, mcr) has no portable C equivalent"
#endif
