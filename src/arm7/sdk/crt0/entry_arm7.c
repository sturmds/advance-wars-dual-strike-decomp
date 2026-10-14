#include "types.h"
#include "sdk/os_arm7.h"

/* module parameters the linker fills in */
const u32 BuildInfo_Arm7[2] = {
#include "data/arm7/BuildInfo_Arm7.inc.c"
};
void do_autoload_Arm7(void);
void DetectMainMemorySize(void);
void Arm7Main(void);

#ifdef __MWERKS__
/*
 * ARM7 entry point from the cartridge header (crt0). Turns interrupts off
 * through IME and clears ARM7 WRAM from 0x03800000 (or from the end of this
 * crt0, 0x02380170, if that were higher) up to the stacks at 0x0380ff00.
 * Gives the supervisor, IRQ and system modes their stacks, copies the
 * autoload blocks, clears .bss, records the main RAM size, stores the IRQ
 * handler in the WRAM IRQ vector at 0x0380fffc and jumps to the ARM7 main
 * with lr at the BIOS reset vector 0xffff0000.
 */
asm void Entry_Arm7(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    ldr r1, =0x02380170
    mov r0, #0x3800000
    cmp r0, r1
    movpl r1, r0
    ldr r2, =0x0380ff00
    mov r0, #0
clear_wram:
    cmp r1, r2
    stmltia r1!, {r0}
    blt clear_wram
    mov r0, #0x13
    msr CPSR_c, r0
    ldr sp, =0x0380ffc0
    mov r0, #0x12
    msr CPSR_c, r0
    ldr r0, =0x0380ff80
    mov sp, r0
    ldr r1, =0x400
    sub r1, r0, r1
    mov r0, #0x1f
    msr CPSR_fsxc, r0
    sub sp, r1, #4
    bl do_autoload_Arm7
    ldr r0, =BuildInfo_Arm7
    ldr r1, [r0, #0xc]
    ldr r2, [r0, #0x10]
    mov r0, #0
clear_bss:
    cmp r1, r2
    strcc r0, [r1], #4
    bcc clear_bss
    bl DetectMainMemorySize
    ldr r1, =0x0380fffc
    ldr r0, =Irq_DispatchLowestPending_Arm7
    str r0, [r1]
    ldr r1, =Arm7Main
    ldr lr, =0xffff0000
    bx r1
}
#else
#error "Entry_Arm7: crt0 stack set-up in supervisor, IRQ and system modes (msr) has no portable C equivalent"
#endif
