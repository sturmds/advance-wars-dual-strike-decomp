#include "types.h"

/* Returns the processor mode bits of CPSR. */
asm u32 OS_GetProcMode(void)
{
    mrs r0, CPSR
    and r0, r0, #0x1f
    bx lr
}
