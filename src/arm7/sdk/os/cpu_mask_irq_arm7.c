#include "types.h"
#include "sdk/os_arm7.h"

#ifdef __MWERKS__
asm int Cpu_MaskIrq_Arm7(void)
{
    mrs r0, CPSR
    orr r1, r0, #0x80
    msr CPSR_c, r1
    and r0, r0, #0x80
    bx lr
}
#else
#error "Cpu_MaskIrq_Arm7: setting the IRQ-disable bit in CPSR (mrs/msr) has no portable C equivalent"
#endif
