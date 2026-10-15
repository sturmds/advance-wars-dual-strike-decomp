#include "types.h"
#include "sdk/gx_load.h"

/* Copies size bytes in 32-bit units. */
void Mem_Copy32(const void* src, void* dst, u32 size) {
  MIi_CpuCopy32(src, dst, size);
}
