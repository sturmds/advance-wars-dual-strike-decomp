#include "types.h"

asm u32 Cpu_CurrentMode_Arm7(void)
{
    mrs r0, CPSR
    and r0, r0, #0x1f
    bx lr
}
