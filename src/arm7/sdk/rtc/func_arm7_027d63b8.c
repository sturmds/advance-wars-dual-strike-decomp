#include "sdk/rtc.h"

/* Sends the command byte, mode | reg, low bit first: for each bit, clock
 * low with the bit on the data line, wait, clock high, wait. */
asm void func_arm7_027d63b8(int mode, int reg)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    orr r1, r0, r1
    ldrh r0, [r12, #0x0]
    bic r0, r0, #0x77
    orr r0, r0, #0x74
    mov r2, #0x0
bit:
    bic r0, r0, #0x3
    orr r0, r0, #0x0
    mov r3, #0x1
    tst r3, r1, lsr r2
    movne r3, #0x1
    moveq r3, #0x0
    orr r0, r0, r3
    strh r0, [r12, #0x0]
    mov r3, #0x9
wait1:
    subs r3, r3, #0x1
    bne wait1
    bic r0, r0, #0x2
    orr r0, r0, #0x2
    strh r0, [r12, #0x0]
    mov r3, #0x9
wait2:
    subs r3, r3, #0x1
    bne wait2
    add r2, r2, #0x1
    cmp r2, #0x8
    bne bit
    bx lr
}
