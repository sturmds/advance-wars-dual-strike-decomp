#include "sdk/mi.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly: LZ77 unpacker that writes halfwords (strh, safe
 * for VRAM). src is the compressed stream with its 4-byte header, dest the
 * output. Bytes are collected in r3 and stored two at a time; a run copy
 * reads the earlier output a halfword at a time and picks the byte it needs.
 * Written as asm because the conditional strh and the shifted-register
 * merges have no C form that mwcc turns back into these instructions.
 */
asm void Unpack_Lz77ByHalfwords(const u8 *src, u16 *dest)
{
    stmfd sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    mov r3, #0
    ldr r8, [r0], #4
    mov r10, r8, lsr #8
    mov r2, #0
nextBlock:
    cmp r10, #0
    ble done
    ldrb r6, [r0], #1
    mov r7, #8
nextItem:
    subs r7, r7, #1
    blt nextBlock
    tst r6, #0x80
    bne copyRun
    ldrb r9, [r0], #1
    orr r3, r3, r9, lsl r2
    sub r10, r10, #1
    eors r2, r2, #8
    streqh r3, [r1], #2
    moveq r3, #0
    b itemDone
copyRun:
    ldrb r9, [r0]
    mov r8, #3
    add r5, r8, r9, asr #4
    ldrb r9, [r0], #1
    and r8, r9, #0xf
    mov r4, r8, lsl #8
    ldrb r9, [r0], #1
    orr r8, r9, r4
    add r4, r8, #1
    rsb r8, r2, #8
    and r9, r4, #1
    eor lr, r8, r9, lsl #3
    sub r10, r10, r5
copyByte:
    eor lr, lr, #8
    rsb r8, r2, #8
    add r8, r4, r8, lsr #3
    mov r8, r8, lsr #1
    mov r8, r8, lsl #1
    ldrh r9, [r1, -r8]
    mov r8, #0xff
    and r8, r9, r8, lsl lr
    mov r8, r8, asr lr
    orr r3, r3, r8, lsl r2
    eors r2, r2, #8
    streqh r3, [r1], #2
    moveq r3, #0
    subs r5, r5, #1
    bgt copyByte
itemDone:
    cmp r10, #0
    movgt r6, r6, lsl #1
    bgt nextItem
    b nextBlock
done:
    ldmfd sp!, {r4, r5, r6, r7, r8, r9, r10, lr}
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Unpacks the stream with halfword stores, in the format
   MI_UncompressLZ8 reads. Each byte goes into the low, then the high half
   of a halfword, which is stored once both are set. A run byte is read
   from memory through the halfword that holds it, so with a distance of 1
   the low byte of the halfword not yet stored is read from memory, not
   from the byte just unpacked, as in the assembly. A last odd byte is
   never stored, and a run is written whole even past the output size.
   Assumes src is word-aligned and dest halfword-aligned; the loads and
   stores would round them down, so a misaligned one stops the program. */
void Unpack_Lz77ByHalfwords(const u8* src, u16* dest) {
  int remaining;
  u16 halfword = 0;
  int shift = 0;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dest % 2 != 0) {
    fprintf(stderr,
            "Unpack_Lz77ByHalfwords: %p is not word-aligned or %p is not "
            "halfword-aligned\n",
            (const void*)src, (void*)dest);
    abort();
  }
  remaining = src[1] | src[2] << 8 | src[3] << 16;
  src += 4;
  while (remaining > 0) {
    u8 flags = *src++;
    int item;

    for (item = 0; item < 8 && remaining > 0; item++, flags <<= 1) {
      if (flags & 0x80) {
        int length = 3 + (src[0] >> 4);
        int distance = ((src[0] & 0xf) << 8 | src[1]) + 1;

        src += 2;
        remaining -= length;
        do {
          /* The byte distance back from the next output byte, which is
             dest[0]'s high half when shift is 8. */
          int back = distance - shift / 8;
          int fromHigh = back & 1;
          const u16* from = dest - (back + 1) / 2;

          halfword |= (u8)(*from >> (fromHigh ? 8 : 0)) << shift;
          shift ^= 8;
          if (shift == 0) {
            *dest++ = halfword;
            halfword = 0;
          }
        } while (--length > 0);
      } else {
        halfword |= *src++ << shift;
        remaining--;
        shift ^= 8;
        if (shift == 0) {
          *dest++ = halfword;
          halfword = 0;
        }
      }
    }
  }
}
#endif
