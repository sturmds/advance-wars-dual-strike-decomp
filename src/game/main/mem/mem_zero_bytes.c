#include "types.h"

void Mem_ZeroBytes(u8* dest, u32 size) {
  while (size != 0) {
    *dest++ = 0;
    size--;
  }
}
