#include "sdk/rtc.h"

/* Ends a transfer: waits, sets chip select low, and waits again. */
asm void func_arm7_027d6384(void)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
    mov r3, #0x2
wait1:
    subs r3, r3, #0x1
    bne wait1
    ldrh r0, [r12, #0x0]
    bic r0, r0, #0x4
    orr r0, r0, #0x0
    strh r0, [r12, #0x0]
    mov r3, #0x2
wait2:
    subs r3, r3, #0x1
    bne wait2
    bx lr
}
