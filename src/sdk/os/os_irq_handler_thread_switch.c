#include "sdk/os.h"

extern u16 data_027c1278;               /* bit mask of thread ids to wake after the interrupt */
extern void CP_SaveContext(void *coprocessorContext);   /* saves the coprocessor registers */
extern void CPi_RestoreContext(void *coprocessorContext);   /* restores the coprocessor registers */

#ifdef __MWERKS__
/*
 * OS_IrqHandler_ThreadSwitch: the return path of OS_IrqHandler. Marks the
 * threads named in data_027c1278 as ready, then, if the scheduler asked for
 * it, saves the interrupted thread's context, runs the switch callback and
 * resumes the highest priority ready thread. With nothing to switch to it
 * returns to the interrupted code.
 *
 * Offsets used on OSThread: 0x64 state, 0x68 next, 0x6c id.
 */
asm void OS_IrqHandler_ThreadSwitch(void)
{
    ldr r12, =data_027c1278
    ldrh r3, [r12]
    mov r0, #0
    cmp r3, #0
    beq check_resched
    strh r0, [r12]
    ldr r12, =OSi_ThreadInfo
    mov r1, #1
    strh r1, [r12]
    ldr r12, [r12, #12]
    mov r2, #1
wake_loop:
    cmp r12, #0
    beq check_resched
    ldr r0, [r12, #0x6c]
    tst r3, r1, lsl r0
    strne r2, [r12, #0x64]
    ldr r12, [r12, #0x68]
    b wake_loop
check_resched:
    ldr r12, =OSi_ThreadInfo
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
    ldrneh r0, [r1, #0x64]
    cmpne r0, #1
    ldrne r1, [r1, #0x68]
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
    stmfd sp!, {r0, r1}
    add r0, r0, #0
    add r0, r0, #0x48
    ldr r1, =CP_SaveContext
    blx r1
    ldmfd sp!, {r0, r1}
    ldmib sp!, {r2, r3}
    stmib r0!, {r2, r3}
    ldmib sp!, {r2, r3, r12, lr}
    stmib r0!, {r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, sp, lr}^
    stmib r0!, {lr}
    mov r3, #0xd3
    msr CPSR_c, r3
    stmib r0!, {sp}
    stmfd sp!, {r1}
    add r0, r1, #0
    add r0, r0, #0x48
    ldr r1, =CPi_RestoreContext
    blx r1
    ldmfd sp!, {r1}
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
#else
#error "OS_IrqHandler_ThreadSwitch: switching threads from IRQ mode (msr, banked registers, ldm ^) has no portable C equivalent"
#endif
