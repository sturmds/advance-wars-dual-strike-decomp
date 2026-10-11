#include "types.h"

/* Reads the CP15 data TCM region register and returns its base address. */
asm u32 Cp15_DataTcmBase(void)
{
    mrc p15, 0, r0, c9, c1, 0
    ldr r1, =0xfffff000
    and r0, r0, r1
    bx lr
}
