#include "types.h"

/*
 * Hand-written ARM assembly, the ARM7 side of a restart (the ARM9 side is
 * Reset_ClearAndJumpToEntry). Turns interrupts off and clears the IRQ vector at
 * 0x0380fffc, then syncs with the ARM9 through the IPC sync register
 * (0x04000180): sends 1, waits for the ARM9's 1, sends 0 and waits for the
 * ARM9 to leave 1. Jumps to the ARM7 entry address stored in the ROM header
 * copy at 0x027ffe00, with r0-r3 cleared.
 */
asm void Reset_HandshakeAndReboot_Arm7(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    ldr r1, =0x0380fffc
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x04000180
    mov r0, #0x100
    strh r0, [r1]
wait_remote:
    ldrh r0, [r1]
    and r0, r0, #15
    cmp r0, #1
    bne wait_remote
    ldr r1, =0x04000180
    mov r0, #0
    strh r0, [r1]
wait_release:
    ldrh r0, [r1]
    cmp r0, #1
    beq wait_release
    ldr r3, =0x027ffe00
    ldr r12, [r3, #0x34]
    mov lr, r12
    mov r0, #0
    mov r1, #0
    mov r2, #0
    mov r3, #0
    bx r12
}
