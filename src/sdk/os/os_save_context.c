#include "sdk/os.h"

extern void CP_SaveContext(void *coprocessorContext);   /* saves the coprocessor registers */

/*
 * OS_SaveContext: saves CPSR, r0 to r12, sp and lr into context,
 * then the coprocessor state at context + 0x48. Like setjmp, it returns 0
 * now and 1 when OS_LoadContext later resumes the saved context.
 */
asm int OS_SaveContext(OSContext *context)
{
    stmfd sp!, {r0, lr}
    add r0, r0, #0x48
    ldr r1, =CP_SaveContext
    blx r1
    ldmfd sp!, {r0, lr}
    add r1, r0, #0
    mrs r2, CPSR
    str r2, [r1], #4
    mov r0, #0xd3
    msr CPSR_c, r0
    str sp, [r1, #0x40]
    msr CPSR_c, r2
    mov r0, #1
    stmia r1, {r0-r12, sp, lr}
    add r0, pc, #8
    str r0, [r1, #0x3c]
    mov r0, #0
    bx lr
}
