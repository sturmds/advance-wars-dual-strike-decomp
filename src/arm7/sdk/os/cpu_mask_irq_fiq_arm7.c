#include "types.h"
#include "sdk/os_arm7.h"

asm OSIntrMode Cpu_MaskIrqFiq_Arm7(void)
{
    mrs r0, CPSR
    orr r1, r0, #0xc0
    msr CPSR_c, r1
    and r0, r0, #0xc0
    bx lr
}
