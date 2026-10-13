#include "types.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly: LZ77 unpacker that writes single bytes (swpb,
 * safe for any memory). src is the compressed stream with its 4-byte
 * header, dest the output. Sits in gUnpackFuncs for LZ data read from main
 * RAM. Written as asm because the swpb stores have no C form.
 */
asm void MI_UncompressLZ8(const u8 *src, u8 *dest)
{
    stmfd sp!, {r4, r5, r6, lr}
    ldr r5, [r0], #4
    mov r2, r5, lsr #8
nextBlock:
    cmp r2, #0
    ble done
    ldrb lr, [r0], #1
    mov r4, #8
nextItem:
    subs r4, r4, #1
    blt nextBlock
    tst lr, #0x80
    bne copyRun
    ldrb r6, [r0], #1
    swpb r6, r6, [r1]
    add r1, r1, #1
    sub r2, r2, #1
    b itemDone
copyRun:
    ldrb r5, [r0]
    mov r6, #3
    add r3, r6, r5, asr #4
    ldrb r6, [r0], #1
    and r5, r6, #0xf
    mov r12, r5, lsl #8
    ldrb r6, [r0], #1
    orr r5, r6, r12
    add r12, r5, #1
    sub r2, r2, r3
copyByte:
    ldrb r5, [r1, -r12]
    swpb r5, r5, [r1]
    add r1, r1, #1
    subs r3, r3, #1
    bgt copyByte
itemDone:
    cmp r2, #0
    movgt lr, lr, lsl #1
    bgt nextItem
    b nextBlock
done:
    ldmfd sp!, {r4, r5, r6, lr}
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Unpacks the stream with byte stores. The header's upper 24 bits give the
   output size. Each flag byte, read from its top bit down, marks up to
   eight items: a literal byte (bit clear) or a run of 3 + (b0 >> 4) bytes
   copied one at a time from ((b0 & 0xf) << 8 | b1) + 1 bytes back (bit
   set). A run is written whole even past the output size, as in the
   assembly. Assumes src is word-aligned; the header load would round it
   down, so a misaligned one stops the program. */
void MI_UncompressLZ8(const u8* src, u8* dest) {
  int remaining;

  if ((uintptr_t)src % 4 != 0) {
    fprintf(stderr, "MI_UncompressLZ8: %p is not word-aligned\n",
            (const void*)src);
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
          *dest = dest[-distance];
          dest++;
        } while (--length > 0);
      } else {
        *dest++ = *src++;
        remaining--;
      }
    }
  }
}
#endif
