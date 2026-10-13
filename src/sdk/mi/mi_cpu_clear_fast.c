#include "sdk/mi.h"

#ifdef __MWERKS__
/*
 * Hand-written ARM assembly in the SDK: fills 32 bytes per store-multiple,
 * then finishes the remaining words one at a time.
 */
asm void MIi_CpuClearFast(u32 data, void *dest, u32 size)
{
    stmdb sp!, {r4, r5, r6, r7, r8, r9}
    add r9, r1, r2
    mov r12, r2, lsr #5
    add r12, r1, r12, lsl #5
    mov r2, r0
    mov r3, r2
    mov r4, r2
    mov r5, r2
    mov r6, r2
    mov r7, r2
    mov r8, r2
block:
    cmp r1, r12
    stmltia r1!, {r0, r2, r3, r4, r5, r6, r7, r8}
    blt block
word:
    cmp r1, r9
    stmltia r1!, {r0}
    blt word
    ldmia sp!, {r4, r5, r6, r7, r8, r9}
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stores data into each word from dest while the word starts before
   dest + size, so a size that is not a multiple of 4 writes one more word.
   The assembly does whole 32-byte blocks first, which writes the same
   words. Assumes dest is word-aligned; the stores would round it down, so
   a misaligned one stops the program. */
void MIi_CpuClearFast(u32 data, void* dest, u32 size) {
  u32* to = dest;
  u32 i;

  if ((uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "MIi_CpuClearFast: %p is not word-aligned\n", dest);
    abort();
  }
  for (i = 0; i < size; i += 4) {
    *to++ = data;
  }
}
#endif
