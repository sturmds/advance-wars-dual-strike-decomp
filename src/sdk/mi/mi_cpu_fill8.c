#include "sdk/mi.h"

/*
 * Hand-written ARM assembly in the SDK: fills bytes with halfword stores at
 * the unaligned edges and word stores in the middle, since the bus has no
 * byte writes to VRAM.
 */
asm void MI_CpuFill8(void *dest, u32 data, u32 size)
{
    cmp r2, #0
    bxeq lr
    tst r0, #1
    beq aligned16
    ldrh r12, [r0, #-1]
    and r12, r12, #0xff
    orr r3, r12, r1, lsl #8
    strh r3, [r0, #-1]
    add r0, r0, #1
    subs r2, r2, #1
    bxeq lr
aligned16:
    cmp r2, #2
    bcc tail
    orr r1, r1, r1, lsl #8
    tst r0, #2
    beq aligned32
    strh r1, [r0], #2
    subs r2, r2, #2
    bxeq lr
aligned32:
    orr r1, r1, r1, lsl #16
    bics r3, r2, #3
    beq wordsDone
    sub r2, r2, r3
    add r12, r3, r0
words:
    str r1, [r0], #4
    cmp r0, r12
    bcc words
wordsDone:
    tst r2, #2
    strneh r1, [r0], #2
tail:
    tst r2, #1
    bxeq lr
    ldrh r3, [r0]
    and r3, r3, #0xff00
    and r1, r1, #0xff
    orr r1, r1, r3
    strh r1, [r0]
    bx lr
}
