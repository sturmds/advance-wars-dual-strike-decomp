#include "sdk/os.h"

extern void CPi_RestoreContext(void *coprocessorContext);   /* restores the coprocessor registers */

/*
 * OS_LoadContext: restores the coprocessor state, then switches to
 * the supervisor mode context saved by OS_SaveContext and continues where it
 * was saved. The saved r0 is 1, so OS_SaveContext appears to return 1.
 */
asm void OS_LoadContext(OSContext *context)
{
    stmfd sp!, {r0, lr}
    add r0, r0, #0x48
    ldr r1, =CPi_RestoreContext
    blx r1
    ldmfd sp!, {r0, lr}
    mrs r1, CPSR
    bic r1, r1, #0x1f
    orr r1, r1, #0xd3
    msr CPSR_c, r1
    ldr r1, [r0], #4
    msr SPSR_fsxc, r1
    ldr sp, [r0, #0x40]
    ldr lr, [r0, #0x3c]
    ldmia r0, {r0-r12, sp, lr}^
    nop
    subs pc, lr, #4
}
