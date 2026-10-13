#include "types.h"
#include "sdk/os.h"

#ifdef __MWERKS__
/* Puts the IRQ and FIQ disable bits back to the value OS_DisableInterrupts_IrqAndFiq returned. */
asm OSIntrMode OS_RestoreInterrupts_IrqAndFiq(OSIntrMode state)
{
    mrs r1, CPSR
    bic r2, r1, #0xc0
    orr r2, r2, r0
    msr CPSR_c, r2
    and r0, r1, #0xc0
    bx lr
}
#else
#error "OS_RestoreInterrupts_IrqAndFiq: writing the IRQ and FIQ disable bits in CPSR (mrs/msr) has no portable C equivalent"
#endif
