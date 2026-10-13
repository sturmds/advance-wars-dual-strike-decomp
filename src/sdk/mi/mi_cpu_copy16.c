#include "sdk/mi.h"

#ifdef __MWERKS__
/* Hand-written ARM assembly in the SDK: a loop that tests first and copies under a condition. */
asm void MIi_CpuCopy16(const void *src, void *dest, int size)
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

/* Copies halfwords from src to dest, front to back, while the offset is
   below size, so an odd size copies one more halfword and a size of 0 or
   less copies none. Assumes both pointers are halfword-aligned; the loads
   and stores would round them down, so a misaligned one stops the
   program. */
void MIi_CpuCopy16(const void* src, void* dest, int size) {
  const u16* from = src;
  u16* to = dest;
  int i;

  if ((uintptr_t)src % 2 != 0 || (uintptr_t)dest % 2 != 0) {
    fprintf(stderr, "MIi_CpuCopy16: %p or %p is not halfword-aligned\n", src,
            dest);
    abort();
  }
  for (i = 0; i < size; i += 2) {
    *to++ = *from++;
  }
}
#endif
