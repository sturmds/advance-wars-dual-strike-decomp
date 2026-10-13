#include "sdk/mi.h"

#ifdef __MWERKS__
/* Hand-written ARM assembly in the SDK: a loop that tests first and stores under a condition. */
asm void MIi_CpuClear32(u32 data, void *dest, int size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    stmltia r1!, {r0}
    blt loop
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stores data into each word from dest while the word starts before
   dest + size, so a size that is not a multiple of 4 writes one more word
   and a size of 0 or less writes none. Assumes dest is word-aligned; the
   store would round it down, so a misaligned one stops the program. */
void MIi_CpuClear32(u32 data, void* dest, int size) {
  u32* to = dest;
  int i;

  if ((uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "MIi_CpuClear32: %p is not word-aligned\n", dest);
    abort();
  }
  for (i = 0; i < size; i += 4) {
    *to++ = data;
  }
}
#endif
