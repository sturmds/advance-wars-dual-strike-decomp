#include "types.h"

void MI_CpuFill8(void* dst, u32 value, u32 size);    /* SDK byte fill */
void MIi_CpuClear16(u16 value, void* dst, u32 size); /* SDK 16-bit fill */
void MIi_CpuClear32(u32 value, void* dst, u32 size); /* SDK 32-bit fill */

/* Zeroes size bytes in units of unitBits (8, 16 or 32) bits. The first
 * argument is ignored; callers pass 0. Any other unitBits zeroes bytes. The
 * volatile temps are needed to match. */
void Mem_ZeroUnits(u32 ignored, void* dst, u32 size, u32 unitBits) {
  volatile u16 zero16;
  volatile u32 zero32;

  switch (unitBits) {
    default:
    case 8:
      MI_CpuFill8(dst, 0, size);
      break;
    case 16:
      zero16 = 0;
      MIi_CpuClear16(zero16, dst, size);
      break;
    case 32:
      zero32 = 0;
      MIi_CpuClear32(zero32, dst, size);
      break;
  }
}
