#include "sdk/svc.h"

#ifdef __MWERKS__
/* ARM7 BIOS call 0x0c: word copy and fill, 32 bytes at a time. */
#pragma thumb on
asm void Bios_FastFillOrCopy_Arm7(const void *src, void *dst, u32 control)
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
#define FASTSET_COUNT_MASK 0x001fffff /* words to write */
#define FASTSET_FILL 0x01000000       /* write the first source word each time */

/* Writes the word count to dst, copied from src or filled with its first
   word. The BIOS writes whole blocks of 8 words and rounds misaligned
   pointers down, so a count that is not a multiple of 8 or a pointer that
   is not word-aligned stops the program. */
void Bios_FastFillOrCopy_Arm7(const void* src, void* dst, u32 control) {
  u32 count = control & FASTSET_COUNT_MASK;
  const u32* from = src;
  u32* to = dst;
  u32 i;

  if ((uintptr_t)src % 4 != 0 || (uintptr_t)dst % 4 != 0 || count % 8 != 0) {
    fprintf(stderr,
            "Bios_FastFillOrCopy_Arm7: %p or %p is not 4-byte aligned, or "
            "%u words is not a multiple of 8\n",
            src, dst, count);
    abort();
  }
  for (i = 0; i < count; i++) {
    to[i] = (control & FASTSET_FILL) ? from[0] : from[i];
  }
}
#endif
