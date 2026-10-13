#include "sdk/mi.h"

#ifdef __MWERKS__
asm void Mem_FillWords_Arm7(u32 data, void *dest, u32 size)
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
   dest + size, so a size that is not a multiple of 4 writes one more word.
   The assembly compares the two addresses as signed numbers; for any DS
   address that means a size of 0x80000000 or more writes none. Assumes dest
   is word-aligned; the store would round it down, so a misaligned one stops
   the program. */
void Mem_FillWords_Arm7(u32 data, void* dest, u32 size) {
  u32* to = dest;
  s32 i;

  if ((uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "Mem_FillWords_Arm7: %p is not word-aligned\n", dest);
    abort();
  }
  for (i = 0; i < (s32)size; i += 4) {
    *to++ = data;
  }
}
#endif
