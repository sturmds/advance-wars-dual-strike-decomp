#include "sdk/os.h"

extern OSIrqFunction OS_IRQTable[];   /* handler table, one entry per interrupt bit */
void OS_IrqHandler_ThreadSwitch(void);

/*
 * OS_IrqHandler: IRQ vector body. With IME on, picks the lowest bit set in
 * IE & IF, acknowledges it in IF, and calls its handler from the table with
 * lr pointing at OS_IrqHandler_ThreadSwitch. Returns right away when IME is
 * off or no enabled interrupt is pending.
 */
asm void OS_IrqHandler(void)
{
    stmfd sp!, {lr}
    mov r12, #0x4000000
    add r12, r12, #0x210
    ldr r1, [r12, #-8]
    cmp r1, #0
    ldmeqfd sp!, {pc}
    ldmia r12, {r1, r2}
    ands r1, r1, r2
    ldmeqfd sp!, {pc}
    mov r3, #0x80000000
next_bit:
    clz r0, r1
    bics r1, r1, r3, lsr r0
    bne next_bit
    mov r1, r3, lsr r0
    str r1, [r12, #4]
    rsbs r0, r0, #31
    ldr r1, =OS_IRQTable
    ldr r0, [r1, r0, lsl #2]
    ldr lr, =OS_IrqHandler_ThreadSwitch
    bx r0
}
