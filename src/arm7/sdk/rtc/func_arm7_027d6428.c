#include "sdk/rtc.h"

/* Sends `size` bytes from `data`, each low bit first as in the command
 * byte. Bytes are read with halfword loads only. */
asm void func_arm7_027d6428(void *data, int size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
byte:
    stmfd sp!, {r0, r1}
    tst r0, #0x1
    ldreqh r1, [r0, #0x0]
    ldrneh r1, [r0, #-0x1]
    movne r1, r1, lsr #0x8
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
    ldmfd sp!, {r0, r1}
    add r0, r0, #0x1
    subs r1, r1, #0x1
    bne byte
    bx lr
}
