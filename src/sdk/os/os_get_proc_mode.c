#include "types.h"

#ifdef __MWERKS__
/* Returns the processor mode bits of CPSR. */
asm u32 OS_GetProcMode(void)
{
    mrs r0, CPSR
    and r0, r0, #0x1f
    bx lr
}
#else
#error "OS_GetProcMode: reading the mode bits of CPSR (mrs) has no portable C equivalent"
#endif
