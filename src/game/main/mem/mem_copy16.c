#include "types.h"

void Mem_CopyUnits(const void* src, void* dst, u32 size,
                   u32 unitBits); /* copies in 8-, 16- or 32-bit units */

/* Copies size bytes in 16-bit units. */
void Mem_Copy16(const void* src, void* dst, u32 size) {
  Mem_CopyUnits(src, dst, size, 16);
}
