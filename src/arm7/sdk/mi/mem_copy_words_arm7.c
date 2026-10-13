#include "sdk/mi.h"

#ifdef __MWERKS__
asm void Mem_CopyWords_Arm7(const void *src, void *dest, u32 size)
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
   dest + size, so a size that is not a multiple of 4 copies one more word.
   The assembly compares the two addresses as signed numbers; for any DS
   address that means a size of 0x80000000 or more copies none. Assumes both
   pointers are word-aligned; the loads and stores would round them down, so
   a misaligned one stops the program. */
void Mem_CopyWords_Arm7(const void* src, void* dest, u32 size) {
  const u32* from = src;
  u32* to = dest;
  s32 i;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "Mem_CopyWords_Arm7: %p or %p is not word-aligned\n", src,
            dest);
    abort();
  }
  for (i = 0; i < (s32)size; i += 4) {
    *to++ = *from++;
  }
}
#endif
