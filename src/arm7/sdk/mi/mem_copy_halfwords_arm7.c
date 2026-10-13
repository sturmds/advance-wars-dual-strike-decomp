#include "sdk/mi.h"

#ifdef __MWERKS__
asm void Mem_CopyHalfwords_Arm7(const void *src, void *dest, u32 size)
{
    mov r12, #0
loop:
    cmp r12, r2
    ldrlth r3, [r0, r12]
    strlth r3, [r1, r12]
    addlt r12, r12, #2
    blt loop
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Copies halfwords from src to dest while the offset is below size. The
   assembly compares the offset with size as signed numbers, so an odd size
   copies one more halfword and a size of 0x80000000 or more copies none.
   Assumes both pointers are halfword-aligned; the loads and stores would
   round them down, so a misaligned one stops the program. */
void Mem_CopyHalfwords_Arm7(const void* src, void* dest, u32 size) {
  const u16* from = src;
  u16* to = dest;
  s32 i;

  if ((uintptr_t)src % 2 != 0 || (uintptr_t)dest % 2 != 0) {
    fprintf(stderr,
            "Mem_CopyHalfwords_Arm7: %p or %p is not halfword-aligned\n", src,
            dest);
    abort();
  }
  for (i = 0; i < (s32)size; i += 2) {
    *to++ = *from++;
  }
}
#endif
