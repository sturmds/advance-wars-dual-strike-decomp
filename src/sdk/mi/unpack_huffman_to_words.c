#include "sdk/mi.h"

#ifdef __MWERKS__
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
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Unpacks the stream with word stores. Each input bit picks a child of
   the current node (offsets from src): the pair at (node & ~1) +
   (node byte & 0x3f) * 2 + 2, one byte each, and bit 7 (left) or 6 (right)
   of the node byte says that child is a symbol. Symbols enter each word
   from the top, so the first one ends in the low bits; a word holds
   (bits & 7) + 4 symbols, which fills it for 4 or 8 bits. A last partial
   word is never stored. Assumes src is word-aligned and the bit stream
   starts on a word boundary; the loads would round them down, so a
   misaligned one stops the program. */
void Unpack_HuffmanToWords(const u8* src, u32* dest) {
  const u8* stream;
  int bits;
  int perWord;
  int remaining;
  u32 node = 5;
  u32 word = 0;
  int symbols = 0;

  if ((uintptr_t)src % 4 != 0 || (src[4] + 1) * 2 % 4 != 0) {
    fprintf(stderr,
            "Unpack_HuffmanToWords: %p or its bit stream is not "
            "word-aligned\n",
            (const void*)src);
    abort();
  }
  bits = src[0] & 0xf;
  perWord = (bits & 7) + 4;
  remaining = src[1] | src[2] << 8 | src[3] << 16;
  stream = src + 4 + (src[4] + 1) * 2;
  while (remaining > 0) {
    u32 input =
        stream[0] | stream[1] << 8 | stream[2] << 16 | (u32)stream[3] << 24;
    int bit;

    stream += 4;
    for (bit = 0; bit < 32 && remaining > 0; bit++, input <<= 1) {
      u32 right = input >> 31;
      u8 parent = src[node];

      node = (node & ~1u) + (parent & 0x3f) * 2 + 2 + right;
      if ((parent << right) & 0x80) {
        /* The shift by 32 - bits is by 32 when bits is 0, which ARM turns
           into 0 and C leaves undefined. */
        word = (word >> bits) | (bits ? (u32)src[node] << (32 - bits) : 0);
        node = 5;
        if (++symbols == perWord) {
          *dest++ = word;
          remaining -= 4;
          symbols = 0;
        }
      }
    }
  }
}
#endif
