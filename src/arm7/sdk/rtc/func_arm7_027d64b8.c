#include "sdk/rtc.h"

/* Receives `size` bytes into `data`, each low bit first: the data line is
 * turned to input, and each bit is sampled with the clock low. Bytes are
 * stored with halfword read-modify-writes only. */
asm void func_arm7_027d64b8(void *data, int size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
byte:
    stmfd sp!, {r0, r1}
    ldrh r0, [r12, #0x0]
    bic r0, r0, #0x77
    orr r0, r0, #0x64
    mov r2, #0x0
    mov r1, #0x0
bit:
    bic r0, r0, #0x3
    orr r0, r0, #0x0
    strh r0, [r12, #0x0]
    mov r3, #0x9
wait1:
    subs r3, r3, #0x1
    bne wait1
    ldrh r0, [r12, #0x0]
    and r3, r0, #0x1
    cmp r3, #0x1
    moveq r3, #0x80
    movne r3, #0x0
    orr r2, r3, r2, lsr #0x1
    bic r0, r0, #0x2
    orr r0, r0, #0x2
    strh r0, [r12, #0x0]
    mov r3, #0x9
wait2:
    subs r3, r3, #0x1
    bne wait2
    add r1, r1, #0x1
    cmp r1, #0x8
    bne bit
    ldmfd sp!, {r0, r1}
    tst r0, #0x1
    beq even
    ldrh r3, [r0, #-0x1]
    bic r3, r3, #0xff00
    mov r2, r2, lsl #0x8
    orr r3, r2, r3
    strh r3, [r0, #-0x1]
    b next
even:
    ldrh r3, [r0, #0x0]
    bic r3, r3, #0xff
    orr r3, r3, r2
    strh r3, [r0, #0x0]
next:
    add r0, r0, #0x1
    subs r1, r1, #0x1
    bne byte
    bx lr
}
