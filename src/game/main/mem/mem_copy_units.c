#include "types.h"
#include "sdk/gx_load.h"

void Mem_CopyBytes(const void* src, void* dst, u32 size); /* copies bytes */
void MIi_CpuCopy16(const void* src, void* dst,
                   u32 size); /* copies 16-bit units */

/* Copies size bytes in units of unitBits (8, 16 or 32) bits.
 * Any other value copies bytes. */
void Mem_CopyUnits(const void* src, void* dst, u32 size, u32 unitBits) {
  switch (unitBits) {
    default:
    case 8:
      Mem_CopyBytes(src, dst, size);
      break;
    case 16:
      MIi_CpuCopy16(src, dst, size);
      break;
    case 32:
      MIi_CpuCopy32(src, dst, size);
      break;
  }
}
