#include "types.h"

#ifdef __MWERKS__
/*
 * Hand-written Thumb assembly: run-length unpacker that writes 16-bit
 * halfwords, safe for VRAM. Same stream format as Unpack_RunLengthByBytes; bytes are
 * collected into r7 and stored when a halfword is full. Sits in gUnpackFuncs
 * for RL data read from VRAM-safe destinations.
 */
#pragma thumb on
asm void Unpack_RunLengthByHalfwords(const u8 *src, u16 *dest)
{
    push {r4, r5, r6, r7}
    sub sp, #12
    mov r7, #0
    ldmia r0!, {r3}
    lsr r5, r3, #8
    mov r4, #0
nextBlock:
    cmp r5, #0
    ble done
    ldrb r3, [r0, #0]
    str r3, [sp, #4]
    add r0, #1
    ldr r3, [sp, #4]
    lsl r2, r3, #25
    lsr r2, r2, #25
    ldr r6, [sp, #4]
    lsr r3, r6, #8
    bcs repeat
    add r2, #1
    sub r5, r5, r2
literal:
    ldrb r6, [r0, #0]
    lsl r6, r4
    orr r7, r6
    add r0, #1
    mov r3, #8
    eor r4, r3
    bne literalNext
    strh r7, [r1, #0]
    add r1, #2
    mov r7, #0
literalNext:
    sub r2, r2, #1
    bgt literal
    b nextBlock
repeat:
    add r2, #3
    sub r5, r5, r2
    ldrb r6, [r0, #0]
    str r6, [sp, #8]
    add r0, #1
fill:
    ldr r6, [sp, #8]
    lsl r6, r4
    orr r7, r6
    mov r3, #8
    eor r4, r3
    bne fillNext
    strh r7, [r1, #0]
    add r1, #2
    mov r7, #0
fillNext:
    sub r2, r2, #1
    bgt fill
    b nextBlock
done:
    add sp, #12
    pop {r4, r5, r6, r7}
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Unpacks the stream with halfword stores: each byte goes into the low,
   then the high half of a halfword, which is stored once both are set. A
   last odd byte is never stored, and a block is written whole even past
   the output size, as in the assembly. Assumes src is word-aligned and
   dest halfword-aligned; the loads and stores would round them down, so a
   misaligned one stops the program. */
void Unpack_RunLengthByHalfwords(const u8* src, u16* dest) {
  int remaining;
  u16 halfword = 0;
  int shift = 0;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dest % 2 != 0) {
    fprintf(stderr,
            "Unpack_RunLengthByHalfwords: %p is not word-aligned or %p is "
            "not halfword-aligned\n",
            (const void*)src, (void*)dest);
    abort();
  }
  remaining = src[1] | src[2] << 8 | src[3] << 16;
  src += 4;
  while (remaining > 0) {
    u8 control = *src++;
    int repeat = control & 0x80;
    int count = (control & 0x7f) + (repeat ? 3 : 1);
    u8 value = 0;

    remaining -= count;
    if (repeat) {
      value = *src++;
    }
    do {
      halfword |= (repeat ? value : *src++) << shift;
      shift ^= 8;
      if (shift == 0) {
        *dest++ = halfword;
        halfword = 0;
      }
    } while (--count > 0);
  }
}
#endif
