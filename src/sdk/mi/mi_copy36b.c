#include "sdk/mi.h"

#ifdef __MWERKS__
/* Hand-written ARM assembly in the SDK: copies nine words with three load-multiples. */
asm void MI_Copy36B(const void *src, void *dest)
{
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    ldmia r0!, {r2, r3, r12}
    stmia r1!, {r2, r3, r12}
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Copies nine words from src to dest, three at a time; each group is read
   before it is written, as the load-multiples do. Assumes both pointers
   are word-aligned; the loads and stores would round them down, so a
   misaligned one stops the program. */
void MI_Copy36B(const void* src, void* dest) {
  const u32* from = src;
  u32* to = dest;
  int i;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dest % 4 != 0) {
    fprintf(stderr, "MI_Copy36B: %p or %p is not word-aligned\n", src, dest);
    abort();
  }
  for (i = 0; i < 36 / sizeof(u32); i += 3) {
    u32 a = from[i];
    u32 b = from[i + 1];
    u32 c = from[i + 2];

    to[i] = a;
    to[i + 1] = b;
    to[i + 2] = c;
  }
}
#endif
