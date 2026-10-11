#include "types.h"

/* Clears bit 0 of the CP15 control register: turns the memory protection unit off. */
asm void OS_DisableProtectionUnit(void)
{
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #1
    mcr p15, 0, r0, c1, c0, 0
    bx lr
}
