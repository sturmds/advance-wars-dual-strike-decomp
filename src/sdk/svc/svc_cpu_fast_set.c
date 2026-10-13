#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x0c: word copy and fill, 32 bytes at a time. */
#pragma thumb on
asm void SVC_CpuFastSet(const void *src, void *dst, u32 control)
{
    swi 0x0c
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Fields of the control word. */
#define CPUFASTSET_COUNT_MASK 0x001fffff /* words to write */
#define CPUFASTSET_FILL 0x01000000       /* write the first source word each time */

/* Writes the word count to dst, copied from src or filled with its first
   word. The BIOS works in blocks of eight words, so the C takes only a
   count that is a multiple of 8, and word-aligned pointers (the BIOS would
   round them down). Other input stops the program. */
void SVC_CpuFastSet(const void* src, void* dst, u32 control) {
  u32 count = control & CPUFASTSET_COUNT_MASK;
  const u32* from = src;
  u32* to = dst;
  u32 i;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dst % 4 != 0) {
    fprintf(stderr, "SVC_CpuFastSet: %p or %p is not word aligned\n", src,
            dst);
    abort();
  }
  if (count % 8 != 0) {
    fprintf(stderr, "SVC_CpuFastSet: %u words is not a multiple of 8\n",
            (unsigned)count);
    abort();
  }
  for (i = 0; i < count; i++) {
    to[i] = (control & CPUFASTSET_FILL) ? from[0] : from[i];
  }
}
#endif
