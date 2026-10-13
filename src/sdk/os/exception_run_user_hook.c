#include "types.h"

extern u32 data_022a9b7c;
extern u32 data_022a9b80;
extern u8 data_022a9b88[];
extern void OS_EnableProtectionUnit(void);
extern void OS_DisableProtectionUnit(void);

#ifdef __MWERKS__
/*
 * Calls the callback in data_022a9b7c (if set) with the context record and
 * data_022a9b80, in system mode with interrupts off, and the protection unit
 * enabled around the call. Written as assembly: it switches mode with msr
 * and carries sp across the switch.
 */
asm void Exception_RunUserHook(void)
{
    stmdb sp!, {lr}
    sub sp, sp, #4
    ldr r0, =data_022a9b7c
    ldr r0, [r0, #0]
    cmp r0, #0
    addeq sp, sp, #4
    ldmeqia sp!, {lr}
    bxeq lr
    mov r0, sp
    ldr r1, =0x9f
    msr CPSR_fsxc, r1
    mov sp, r0
    bl OS_EnableProtectionUnit
    ldr r1, =data_022a9b80
    ldr r0, =data_022a9b7c
    ldr r1, [r1, #0]
    ldr r2, [r0, #0]
    ldr r0, =data_022a9b88
    blx r2
    bl OS_DisableProtectionUnit
    add sp, sp, #4
    ldmia sp!, {lr}
    bx lr
}
#else
#error "Exception_RunUserHook: switching to system mode and carrying sp across the switch (msr) has no portable C equivalent"
#endif
