#include "sdk/mi.h"

/*
 * Hand-written ARM assembly: Huffman unpacker. src is the compressed stream:
 * a 4-byte header (low nibble the symbol size in bits, upper 24 bits the
 * output size), the tree (its first byte gives its size), then the bit
 * stream in 32-bit words, read from the top bit down. dest receives whole
 * words: symbols are collected in r3 and stored once a word is full.
 * Written in assembly in the original, like Unpack_Lz77ByHalfwords.
 */
asm void Unpack_HuffmanToWords(const u8 *src, u32 *dest)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    sub sp, sp, #4
    add r2, r0, #4
    add r7, r2, #1
    ldrb r10, [r0, #0]
    and r4, r10, #0xf
    mov r3, #0
    mov lr, #0
    and r10, r4, #7
    add r11, r10, #4
    str r11, [sp, #0]
    ldr r10, [r0, #0]
    mov r12, r10, lsr #8
    ldrb r10, [r2, #0]
    add r10, r10, #1
    add r0, r2, r10, lsl #1
    mov r2, r7
nextWord:
    cmp r12, #0
    ble done
    mov r8, #32
    ldr r5, [r0], #4
nextBit:
    subs r8, r8, #1
    blt nextWord
    mov r10, #1
    and r9, r10, r5, lsr #31
    ldrb r6, [r2, #0]
    mov r6, r6, lsl r9
    mov r10, r2, lsr #1
    mov r10, r10, lsl #1
    ldrb r11, [r2, #0]
    and r11, r11, #0x3f
    add r11, r11, #1
    add r10, r10, r11, lsl #1
    add r2, r10, r9
    tst r6, #0x80
    beq checkRemaining
    mov r3, r3, lsr r4
    ldrb r10, [r2, #0]
    rsb r11, r4, #32
    orr r3, r3, r10, lsl r11
    mov r2, r7
    add lr, lr, #1
    ldr r11, [sp, #0]
    cmp lr, r11
    streq r3, [r1], #4
    subeq r12, r12, #4
    moveq lr, #0
checkRemaining:
    cmp r12, #0
    movgt r5, r5, lsl #1
    bgt nextBit
    b nextWord
done:
    add sp, sp, #4
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}
    bx lr
}
