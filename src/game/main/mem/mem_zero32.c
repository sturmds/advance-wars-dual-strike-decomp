#include "types.h"

void MIi_CpuClearFast(u32 value, void* dst, u32 size); /* SDK MI_CpuFill32 */

/* Zeroes size bytes in 32-bit units. The first argument is ignored; callers
 * pass 0. The volatile temp is needed to match. */
void Mem_Zero32(u32 ignored, void* dst, u32 size) {
  volatile u32 zero = 0;

  MIi_CpuClearFast(zero, dst, size);
}
