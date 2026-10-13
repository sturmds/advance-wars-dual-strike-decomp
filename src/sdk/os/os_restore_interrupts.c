#include "types.h"
#include "sdk/os.h"

#ifdef __MWERKS__
/* Puts the IRQ-disable bit back to the value OS_DisableInterrupts returned. */
asm void OS_RestoreInterrupts(int state)
{
    mrs r1, CPSR
    bic r2, r1, #0x80
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #0x80
    bx lr
}
#else
#error "OS_RestoreInterrupts: writing the IRQ-disable bit in CPSR (mrs/msr) has no portable C equivalent"
#endif
