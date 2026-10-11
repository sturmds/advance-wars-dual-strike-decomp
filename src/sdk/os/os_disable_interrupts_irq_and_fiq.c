#include "types.h"
#include "sdk/os.h"

/* Sets the IRQ and FIQ disable bits in CPSR and returns their previous values. */
asm OSIntrMode OS_DisableInterrupts_IrqAndFiq(void)
{
    mrs r0, CPSR
    orr r1, r0, #0xc0
    msr CPSR_c, r1
    and r0, r0, #0xc0
    bx lr
}
