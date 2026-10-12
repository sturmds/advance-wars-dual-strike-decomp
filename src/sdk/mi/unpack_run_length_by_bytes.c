#include "types.h"

/*
 * Hand-written ARM assembly: run-length unpacker that writes single bytes
 * (swpb). Each control byte has a run flag in bit 7 and a count in the low
 * 7 bits: a literal block of count + 1 bytes, or one byte repeated
 * count + 3 times. Sits in gUnpackFuncs for RL data read from main RAM.
 */
asm void Unpack_RunLengthByBytes(const u8 *src, u8 *dest)
{
    stmfd sp!, {r4, r5, r7}
    ldmia r0!, {r3}
    mov r7, r3, lsr #8
nextBlock:
    cmp r7, #0
    ble done
    ldrb r4, [r0], #1
    ands r2, r4, #0x7f
    tst r4, #0x80
    bne repeat
    add r2, r2, #1
    sub r7, r7, r2
literal:
    ldrb r3, [r0], #1
    swpb r3, r3, [r1]
    add r1, r1, #1
    subs r2, r2, #1
    bgt literal
    b nextBlock
repeat:
    add r2, r2, #3
    sub r7, r7, r2
    ldrb r5, [r0], #1
fill:
    swpb r4, r5, [r1]
    add r1, r1, #1
    subs r2, r2, #1
    bgt fill
    b nextBlock
done:
    ldmfd sp!, {r4, r5, r7}
    bx lr
}
