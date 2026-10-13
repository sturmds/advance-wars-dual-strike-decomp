#include "sdk/mi.h"

#ifdef __MWERKS__
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
#else
#include <stdint.h>
#include <string.h>

/* Writes one byte by reading and writing the halfword that holds it, so
   dest memory sees only halfword accesses, as in the assembly. */
static void StoreByteInHalfword(u8* at, u8 value) {
  u16* halfword = (u16*)((uintptr_t)at & ~(uintptr_t)1);
  u16 both = *halfword;

  ((u8*)&both)[(uintptr_t)at & 1] = value;
  *halfword = both;
}

/* Writes the first 2 or 4 bytes of pattern with one halfword or word
   store. */
static void StoreUnit(u8* at, const u8* pattern, int size) {
  if (size == 2) {
    u16 halfword;

    memcpy(&halfword, pattern, 2);
    *(u16*)at = halfword;
  } else {
    u32 word;

    memcpy(&word, pattern, 4);
    *(u32*)at = word;
  }
}

/* Fills size bytes from dest. The assembly widens data by ORing it with
   itself shifted by 8, then by 16, so only a data value below 0x100 gives
   a plain byte fill: an odd first byte and a lone last byte get data's low
   byte, the halfword and word stores between them the bytes of the widened
   value. The C keeps that, and the order of the stores. */
void MI_CpuFill8(void* dest, u32 data, u32 size) {
  u8* to = dest;
  u32 halves = data | data << 8;
  u32 words = halves | halves << 16;
  u8 pattern[4];
  u8* end;
  int i;

  for (i = 0; i < 4; i++) {
    pattern[i] = (u8)(words >> (8 * i));
  }
  if (size != 0 && (uintptr_t)to % 2 != 0) {
    StoreByteInHalfword(to++, (u8)data);
    size--;
  }
  if (size >= 2) {
    if ((uintptr_t)to % 4 != 0) {
      StoreUnit(to, pattern, 2);
      to += 2;
      size -= 2;
    }
    end = to + (size & ~3u);
    while (to < end) {
      StoreUnit(to, pattern, 4);
      to += 4;
    }
    if (size & 2) {
      StoreUnit(to, pattern, 2);
      to += 2;
    }
  }
  if (size & 1) {
    StoreByteInHalfword(to, (u8)data);
  }
}
#endif
