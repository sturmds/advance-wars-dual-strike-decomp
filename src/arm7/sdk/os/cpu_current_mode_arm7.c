#include "types.h"

#ifdef __MWERKS__
asm u32 Cpu_CurrentMode_Arm7(void)
{
    mrs r0, CPSR
    and r0, r0, #0x1f
    bx lr
}
#else
#error "Cpu_CurrentMode_Arm7: reading the CPU mode bits of CPSR (mrs) has no portable C equivalent"
#endif
