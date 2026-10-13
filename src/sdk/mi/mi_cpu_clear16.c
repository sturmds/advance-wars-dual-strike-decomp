#include "sdk/mi.h"

#ifdef __MWERKS__
/* Hand-written ARM assembly in the SDK: a loop that tests first and stores under a condition. */
asm void MIi_CpuClear16(u16 data, void *dest, int size)
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

/* Stores data into each halfword from dest while its offset is below size,
   so an odd size writes one more halfword and a size of 0 or less writes
   none. Assumes dest is halfword-aligned; the store would round it down,
   so a misaligned one stops the program. */
void MIi_CpuClear16(u16 data, void* dest, int size) {
  u16* to = dest;
  int i;

  if ((uintptr_t)dest % 2 != 0) {
    fprintf(stderr, "MIi_CpuClear16: %p is not halfword-aligned\n", dest);
    abort();
  }
  for (i = 0; i < size; i += 2) {
    *to++ = data;
  }
}
#endif
