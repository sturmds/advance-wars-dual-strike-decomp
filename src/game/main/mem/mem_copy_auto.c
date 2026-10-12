#include "types.h"

void Mem_CopyUnits(const void* src, void* dst, u32 size,
                   u32 unitBits); /* copies in 8-, 16- or 32-bit units */
void Mem_Copy32(const void* src, void* dst, u32 size);

/* Copies in 32-bit units when size is a multiple of 32 bytes, else in 16-bit units. */
void Mem_CopyAuto(const void* src, void* dst, u32 size) {
  if (size & 31) {
    Mem_CopyUnits(src, dst, size, 16);
  } else {
    Mem_Copy32(src, dst, size);
  }
}
