#include "sdk/mi.h"

#ifdef __MWERKS__
asm void Mem_FillHalfwords_Arm7(u16 data, void *dest, u32 size)
{
    mov r3, #0
loop:
    cmp r3, r2
    strlth r0, [r1, r3]
    addlt r3, r3, #2
    blt loop
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Stores data into each halfword from dest while the offset is below size.
   The assembly compares the offset with size as signed numbers, so an odd
   size writes one more halfword and a size of 0x80000000 or more writes
   none. Assumes dest is halfword-aligned; the store would round it down, so
   a misaligned one stops the program. */
void Mem_FillHalfwords_Arm7(u16 data, void* dest, u32 size) {
  u16* to = dest;
  s32 i;

  if ((uintptr_t)dest % 2 != 0) {
    fprintf(stderr, "Mem_FillHalfwords_Arm7: %p is not halfword-aligned\n",
            dest);
    abort();
  }
  for (i = 0; i < (s32)size; i += 2) {
    *to++ = data;
  }
}
#endif
