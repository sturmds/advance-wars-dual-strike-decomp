#include "types.h"
#include "sdk/os.h"

#ifdef __MWERKS__
/* Sets the IRQ-disable bit in CPSR and returns the previous value of that bit. */
asm int OS_DisableInterrupts(void)
{
    mrs r0, CPSR
    orr r1, r0, #0x80
    msr CPSR_c, r1
    and r0, r0, #0x80
    bx lr
}
#else
#error "OS_DisableInterrupts: setting the IRQ-disable bit in CPSR (mrs/msr) has no portable C equivalent"
#endif
