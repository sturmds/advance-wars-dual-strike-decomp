#include "types.h"
#include "sdk/os_arm7.h"

/*
 * ARM7 copy of OS_SaveContext (main already has that symbol): saves CPSR,
 * r0 to r12, sp and lr into context, plus the supervisor sp. Like setjmp,
 * it returns 0 now and 1 when the saved context is loaded later.
 */
asm int func_arm7_037fc2d4(OSContext *context)
{
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
