#include "types.h"
#include "sdk/os_arm7.h"

/*
 * ARM7 copy of OS_LoadContext (main already has that symbol): switches to
 * supervisor mode and resumes the context that OS_SaveContext saved. The
 * saved r0 is 1, so OS_SaveContext appears to return 1.
 */
asm void func_arm7_037fc308(OSContext *context)
{
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
