#include "types.h"
#include "sdk/module_params.h"

extern char DTCM_BASE[]; /* start of DTCM (ARM9_LAYOUT_SYMBOLS) */
void OS_IrqHandler(void);
void main(void);
void func_02000900(u32 data, void *dest, int size);
void MIi_UncompressBackward(void *bottom);
void do_autoload(void);
void init_cp15(void);
void NitroStartUp(void);
void FpStatus_Reset(void);
void __call_static_initializers(void);

#ifdef __MWERKS__
/*
 * ARM9 entry point from the cartridge header (crt0). Turns interrupts off
 * through IME, sets up the protection unit and caches, then gives the IRQ,
 * supervisor and system modes their stacks at the top of DTCM. Clears DTCM,
 * palette RAM and OAM, decompresses and autoloads the static module, clears
 * .bss and the shared RAM word at 0x027fff9c, stores OS_IrqHandler in the
 * DTCM IRQ vector, runs the C runtime setup and static constructors, and
 * jumps to main with lr at the BIOS reset vector 0xffff0000.
 */
asm void Entry(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    bl init_cp15
    mov r0, #0x13
    msr CPSR_c, r0
    ldr r0, =DTCM_BASE
    add r0, r0, #0x3fc0
    mov sp, r0
    mov r0, #0x12
    msr CPSR_c, r0
    ldr r0, =DTCM_BASE
    add r0, r0, #0x3fc0
    sub r0, r0, #0x40
    mov sp, r0
    ldr r1, =0x400
    sub r1, r0, r1
    mov r0, #0x1f
    msr CPSR_fsxc, r0
    sub sp, r1, #4
    mov r0, #0
    ldr r1, =DTCM_BASE
    mov r2, #0x4000
    bl func_02000900
    mov r0, #0
    ldr r1, =0x05000000
    mov r2, #0x400
    bl func_02000900
    mov r0, #0x200
    ldr r1, =0x07000000
    mov r2, #0x400
    bl func_02000900
    ldr r1, =BuildInfo
    ldr r0, [r1, #0x14]
    bl MIi_UncompressBackward
    bl do_autoload
    ldr r0, =BuildInfo
    ldr r1, [r0, #0xc]
    ldr r2, [r0, #0x10]
    mov r0, #0
clear_bss:
    cmp r1, r2
    strcc r0, [r1], #4
    bcc clear_bss
    ldr r1, =0x027fff9c
    str r0, [r1]
    ldr r1, =DTCM_BASE
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    ldr r0, =OS_IrqHandler
    str r0, [r1]
    bl FpStatus_Reset
    bl NitroStartUp
    bl __call_static_initializers
    ldr r1, =main
    ldr lr, =0xffff0000
    bx r1
}
#else
#error "Entry: crt0 stack set-up in IRQ, supervisor and system modes (msr) has no portable C equivalent"
#endif
