#include "sdk/svc.h"

#ifdef __MWERKS__
/* BIOS call 0x0b: halfword or word copy and fill. */
#pragma thumb on
asm void SVC_CpuSet(const void *src, void *dst, u32 control)
{
    swi 0x0b
    bx lr
}
#pragma thumb reset
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Fields of the control word. */
#define CPUSET_COUNT_MASK 0x001fffff /* units to write */
#define CPUSET_FILL 0x01000000       /* write the first source unit each time */
#define CPUSET_32BIT 0x04000000      /* words instead of halfwords */

/* Writes the unit count of halfwords or words to dst, copied from src or
   filled with its first unit. Assumes both pointers are aligned to the
   unit size; the BIOS would round them down, so a misaligned one stops the
   program. */
void SVC_CpuSet(const void* src, void* dst, u32 control) {
  u32 count = control & CPUSET_COUNT_MASK;
  u32 unit = (control & CPUSET_32BIT) ? 4 : 2;
  u32 i;

  if ((uintptr_t)src % unit != 0 || (uintptr_t)dst % unit != 0) {
    fprintf(stderr, "SVC_CpuSet: %p or %p is not %u-byte aligned\n", src, dst,
            unit);
    abort();
  }
  if (control & CPUSET_32BIT) {
    const u32* from = src;
    u32* to = dst;
    for (i = 0; i < count; i++) {
      to[i] = (control & CPUSET_FILL) ? from[0] : from[i];
    }
  } else {
    const u16* from = src;
    u16* to = dst;
    for (i = 0; i < count; i++) {
      to[i] = (control & CPUSET_FILL) ? from[0] : from[i];
    }
  }
}
#endif
