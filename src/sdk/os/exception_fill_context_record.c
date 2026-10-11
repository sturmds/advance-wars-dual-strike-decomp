#include "types.h"

extern u8 data_022a9b88[];

/*
 * Called from func_0210e6f8 with ip pointing at the frame OSi_ExceptionHandler
 * pushed. Fills the context record data_022a9b88 from that frame, from r4-r11
 * and from the status registers. Takes its inputs in r0 and ip, so it is
 * assembly.
 */
asm void Exception_FillContextRecord(void)
{
    ldr r1, =data_022a9b88
    mrs r2, CPSR
    str r2, [r1, #0x74]
    str r0, [r1, #0x6c]
    ldr r0, [ip, #0]
    str r0, [r1, #4]
    ldr r0, [ip, #4]
    str r0, [r1, #8]
    ldr r0, [ip, #8]
    str r0, [r1, #12]
    ldr r0, [ip, #12]
    str r0, [r1, #16]
    ldr r2, [ip, #16]
    bic r2, r2, #1
    add r0, r1, #20
    stmia r0, {r4, r5, r6, r7, r8, r9, r10, r11}
    str ip, [r1, #0x70]
    ldr r0, [r2, #0]
    str r0, [r1, #0x64]
    ldr r3, [r2, #4]
    str r3, [r1, #0]
    ldr r0, [r2, #8]
    str r0, [r1, #0x34]
    ldr r0, [r2, #12]
    str r0, [r1, #0x40]
    mrs r0, CPSR
    orr r3, r3, #0x80
    bic r3, r3, #0x20
    msr CPSR_fsxc, r3
    str sp, [r1, #0x38]
    str lr, [r1, #0x3c]
    mrs r2, SPSR
    str r2, [r1, #0x7c]
    msr CPSR_fsxc, r0
    bx lr
}
