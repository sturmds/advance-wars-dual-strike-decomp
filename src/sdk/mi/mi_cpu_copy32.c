#include "sdk/mi.h"

#ifdef __MWERKS__
/* Hand-written ARM assembly in the SDK: a loop that tests first and copies under a condition. */
asm void MIi_CpuCopy32(const void *src, void *dest, int size)
{
    add r12, r1, r2
loop:
    cmp r1, r12
    ldmltia r0!, {r2}
    stmltia r1!, {r2}
    blt loop
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Copies words from src to dest while the destination word starts before
   dest + size, so a size that is not a multiple of 4 copies one more word
   and a size of 0 or less copies none. Assumes both pointers are
   word-aligned; the loads and stores would round them down, so a misaligned
   one stops the program. */
void MIi_CpuCopy32(const void* src, void* dest, int size) {
  const u32* from = src;
  u32* to = dest;
  int i;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "MIi_CpuCopy32: %p or %p is not word-aligned\n", src,
            dest);
    abort();
  }
  for (i = 0; i < size; i += 4) {
    *to++ = *from++;
  }
}
#endif
