#include "types.h"

extern u32 data_027c0000;
void Mem_RepeatWordAsm(u32 value, u32 *dest, u32 size);

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly, the last step of a restart. Turns interrupts
 * off, syncs with the ARM7 through the IPC sync register (0x04000180),
 * clears two 0x80-byte blocks near the top of main RAM (keeping the word at
 * 0x027ffd9c), reloads r0-r10 from the second block, then jumps to the
 * ARM9 entry address stored in the ROM header copy at 0x027ffe00.
 */
asm void Reset_ClearAndJumpToEntry(void)
{
    mov r12, #0x4000000
    str r12, [r12, #0x208]
    ldr r1, =data_027c0000
    add r1, r1, #0x3fc0
    add r1, r1, #0x3c
    mov r0, #0
    str r0, [r1]
    ldr r1, =0x04000180
wait_remote:
    ldrh r0, [r1]
    and r0, r0, #15
    cmp r0, #1
    bne wait_remote
    mov r0, #0x100
    strh r0, [r1]
    mov r0, #0
    ldr r3, =0x027ffd9c
    ldr r4, [r3]
    ldr r1, =0x027ffd80
    mov r2, #0x80
    bl Mem_RepeatWordAsm
    str r4, [r3]
    ldr r1, =0x027fff80
    mov r2, #0x80
    bl Mem_RepeatWordAsm
    ldr r1, =0x04000180
wait_release:
    ldrh r0, [r1]
    and r0, r0, #15
    cmp r0, #1
    beq wait_release
    mov r0, #0
    strh r0, [r1]
    ldr r3, =0x027ffe00
    ldr r12, [r3, #0x24]
    mov lr, r12
    ldr r11, =0x027fff80
    ldmia r11, {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10}
    mov r11, #0
    bx r12
}
#else
#error "Reset_ClearAndJumpToEntry: loading r0-r10 from memory and jumping to the ARM9 entry address has no portable C equivalent"
#endif
