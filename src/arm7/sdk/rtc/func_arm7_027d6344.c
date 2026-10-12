#include "sdk/rtc.h"

/* Starts a transfer: all three lines to output with the clock high, then
 * chip select high. Each step waits a short busy loop. */
asm void func_arm7_027d6344(void)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    ldrh r0, [r12, #0x0]
    bic r0, r0, #0x77
    orr r0, r0, #0x72
    strh r0, [r12, #0x0]
    mov r3, #0x2
wait1:
    subs r3, r3, #0x1
    bne wait1
    bic r0, r0, #0x4
    orr r0, r0, #0x4
    strh r0, [r12, #0x0]
    mov r3, #0x2
wait2:
    subs r3, r3, #0x1
    bne wait2
    bx lr
}
