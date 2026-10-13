#include "sdk/os_arm7.h"

OSiNestedIrqWork gOsNestedIrqWork;

extern OSThreadInfo data_arm7_0380790c;
extern u32 gOsNestedIrqPriorities[][2];    /* {IE mask, handler index} pairs, highest priority first */
extern void (*data_arm7_03807750[])(void);

#ifdef __MWERKS__
/*
 * IRQ vector body that lets interrupts nest. Returns at once when IME is off
 * or no enabled interrupt is pending; when none of the pending ones is in
 * 0x01df3fff, it acknowledges them and returns. Otherwise it saves the IRQ
 * state, switches to system mode and, at the outermost level, to its own
 * stack. It takes the first entry of the priority table that is pending,
 * acknowledges it and calls its handler.
 * For any entry but the first, and only at the outermost level, IE is cut
 * down to the first entry's mask and IRQs are turned back on while the
 * handler runs. Afterwards it keeps the IE bits the handler changed,
 * restores the rest, unwinds the stack and runs the thread switch
 * (Irq_WakeAndSwitchOnExit_Arm7) when it leaves the outermost level.
 * Mode words: 0x9f system mode with IRQs off, 0x1f system mode, 0x92 IRQ
 * mode with IRQs off.
 */
asm void IrqNest_DispatchByPriority(void)
{
    mov r12, #0x4000000
    add r1, r12, #0x208
    ldrh r0, [r1]
    tst r0, r0
    bxeq lr
    ldr r3, [r12, #0x210]
    ldr r1, [r12, #0x214]
    ands r2, r1, r3
    bxeq lr
    ldr r0, =0x01df3fff
    tst r2, r0
    streq r2, [r12, #0x214]
    bxeq lr
    stmfd sp!, {lr}
    mrs r0, SPSR
    stmfd sp!, {r0}
    stmdb sp, {sp, lr}^
    sub sp, sp, #8
    mov r0, #0x9f
    msr CPSR_c, r0
    ldr r1, =data_arm7_0380790c
    ldrh r0, [r1, #4]
    add r0, r0, #1
    strh r0, [r1, #4]
    ldr r1, =gOsNestedIrqWork
    cmp r0, #1
    moveq r0, sp
    ldreq sp, [r1, #4]
    streq r0, [r1, #4]
    stmfd sp!, {r3}
    ldr r1, =gOsNestedIrqPriorities
    ldr r0, [r1]
    tst r0, r2
    strne r0, [r12, #0x214]
    ldrne r0, [r1, #4]
    ldrne r3, =data_arm7_03807750
    ldrne r0, [r3, r0, lsl #2]
    bne call_handler
    mov r3, #1
next_entry:
    ldr r0, [r1, r3, lsl #3]
    tst r0, r2
    addeq r3, r3, #1
    beq next_entry
    str r0, [r12, #0x214]
    add r0, r1, r3, lsl #3
    ldr r2, [r0, #4]
    ldr r3, =data_arm7_03807750
    ldr r0, [r3, r2, lsl #2]
    ldr r2, =data_arm7_0380790c
    ldrh r3, [r2, #4]
    cmp r3, #1
    ldreq r2, [r1]
    streq r2, [r12, #0x210]
    moveq r2, #0x1f
    msreq CPSR_c, r2
call_handler:
    ldr r1, [r12, #0x210]
    stmfd sp!, {r1}
    add lr, pc, #0
    bx r0
    mov r0, #0x9f
    msr CPSR_c, r0
    mov r12, #0x4000000
    ldmfd sp!, {r0}
    ldr r1, [r12, #0x210]
    eor r2, r0, r1
    and r1, r2, r1
    and r0, r2, r0
    ldmfd sp!, {r3}
    orr r3, r3, r1
    bic r3, r3, r0
    str r3, [r12, #0x210]
    ldr r2, =data_arm7_0380790c
    ldr r3, =gOsNestedIrqWork
    ldrh r0, [r2, #4]
    subs r1, r0, #1
    strh r1, [r2, #4]
    moveq r0, sp
    ldreq sp, [r3, #4]
    streq r0, [r3, #4]
    mov r0, #0x92
    msr CPSR_c, r0
    ldmia sp, {sp, lr}^
    mov r0, r0
    add sp, sp, #8
    ldmfd sp!, {r0}
    msr SPSR_fc, r0
    tst r1, r1
    ldreq r0, =Irq_WakeAndSwitchOnExit_Arm7
    addeq lr, pc, #0
    bxeq r0
    ldmfd sp!, {pc}
}
#else
#error "IrqNest_DispatchByPriority: nested IRQ dispatch with CPU mode switches, SPSR saves and a stack swap has no portable C equivalent"
#endif
