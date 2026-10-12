#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;   /* ARM7 OSi_ThreadInfo */

/*
 * ARM7 copy of OS_IrqHandler_ThreadSwitch, the return path of
 * func_arm7_037fb340. Marks the threads named in the wake mask
 * data_arm7_03807888 as ready, then, if the scheduler asked for it, saves the
 * interrupted thread's context, runs the switch callback and resumes the
 * highest priority ready thread. With nothing to switch to it returns to the
 * interrupted code. Unlike ARM9 there is no coprocessor context to save.
 *
 * Offsets used on the ARM7 OSThread: 0x48 state, 0x4c next, 0x50 id.
 */
asm void func_arm7_037fb394(void)
{
    ldr r12, =data_arm7_03807888
    ldrh r3, [r12]
    mov r0, #0
    cmp r3, #0
    beq check_resched
    strh r0, [r12]
    ldr r12, =data_arm7_0380790c
    mov r1, #1
    strh r1, [r12]
    ldr r12, [r12, #12]
    mov r2, #1
wake_loop:
    cmp r12, #0
    beq check_resched
    ldr r0, [r12, #0x50]
    tst r3, r1, lsl r0
    strne r2, [r12, #0x48]
    ldr r12, [r12, #0x4c]
    b wake_loop
check_resched:
    ldr r12, =data_arm7_0380790c
    ldrh r1, [r12]
    cmp r1, #0
    ldreq pc, [sp], #4
    mov r1, #0
    strh r1, [r12]
    mov r3, #0xd2
    msr CPSR_c, r3
    add r2, r12, #12
    ldr r1, [r2]
find_ready:
    cmp r1, #0
    ldrneh r0, [r1, #0x48]
    cmpne r0, #1
    ldrne r1, [r1, #0x4c]
    bne find_ready
    cmp r1, #0
    bne found_thread
return_to_irq:
    mov r3, #0x92
    msr CPSR_c, r3
    ldr pc, [sp], #4
found_thread:
    ldr r0, [r12, #8]
    cmp r1, r0
    beq return_to_irq
    ldr r3, [r12, #16]
    cmp r3, #0
    beq no_callback
    stmfd sp!, {r0, r1, r12}
    mov lr, pc
    bx r3
    ldmfd sp!, {r0, r1, r12}
no_callback:
    str r1, [r12, #8]
    mrs r2, SPSR
    str r2, [r0, #0]!
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, r12, lr}
    stmib r0!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr}^
    stmib r0!, {lr}
    mov r3, #0xd3
    msr CPSR_c, r3
    stmib r0!, {sp}
    ldr sp, [r1, #0x44]
    mov r3, #0xd2
    msr CPSR_c, r3
    ldr r2, [r1, #0]!
    msr SPSR_fc, r2
    ldr lr, [r1, #0x40]
    ldmib r1!, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr}^
    nop
    stmda sp!, {r0, r1, r2, r3, r12, lr}
    ldmfd sp!, {pc}
}
