#include "sdk/mi.h"

#ifdef __MWERKS__
asm void Mem_FillBytes_Arm7(void *dest, u32 data, u32 size)
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
#include <stdio.h>
#include <stdlib.h>

/* The DS is little-endian, and StoreByte counts on it. */
static void CheckLittleEndian(const char* name) {
  const u16 one = 1;

  if (*(const u8*)&one != 1) {
    fprintf(stderr, "%s: StoreByte assumes a little-endian CPU\n", name);
    abort();
  }
}

/* Writes value into the byte at p by rewriting the halfword that holds it,
   as the assembly does: VRAM ignores byte stores. */
static void StoreByte(u8* p, u8 value) {
  u16* halfword = (u16*)((uintptr_t)p & ~(uintptr_t)1);
  u32 shift = ((uintptr_t)p & 1) * 8;

  *halfword = (u16)((*halfword & ~(0xff << shift)) | value << shift);
}

/* Stores data into size bytes from dest, with halfword accesses only.
   Assumes data fits in a byte, as every caller's does: the assembly spreads
   the whole of data over its halfword and word stores, so higher bits would
   end up in some bytes and not in others. A larger data stops the program. */
void Mem_FillBytes_Arm7(void* dest, u32 data, u32 size) {
  u8* to = dest;
  u32 i;

  CheckLittleEndian("Mem_FillBytes_Arm7");
  if (data > 0xff) {
    fprintf(stderr, "Mem_FillBytes_Arm7: data 0x%x does not fit in a byte\n",
            (unsigned)data);
    abort();
  }
  for (i = 0; i < size; i++) {
    StoreByte(to + i, (u8)data);
  }
}
#endif
