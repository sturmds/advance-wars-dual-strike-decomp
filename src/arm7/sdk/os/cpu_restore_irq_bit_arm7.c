#include "types.h"
#include "sdk/os_arm7.h"

#ifdef __MWERKS__
asm int Cpu_RestoreIrqBit_Arm7(int state)
{
    mrs r1, CPSR
    bic r2, r1, #0x80
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #0x80
    bx lr
}
#else
#error "Cpu_RestoreIrqBit_Arm7: restoring the IRQ-disable bit in CPSR (mrs/msr) has no portable C equivalent"
#endif
