#include "types.h"

void MIi_CpuCopy32(const void* src, void* dst, u32 size); /* SDK MI_CpuCopy32 */

/* Copies size bytes in 32-bit units. */
void Mem_Copy32(const void* src, void* dst, u32 size) {
  MIi_CpuCopy32(src, dst, size);
}
