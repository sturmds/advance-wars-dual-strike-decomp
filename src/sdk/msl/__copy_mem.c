#include "types.h"
#include "sdk/msl.h"

void* __copy_mem(void* dest, const void* src, u32 count) {
  u8* d = dest;
  const u8* s = src;

  while (count != 0) {
    *d++ = *s++;
    count--;
  }
  return dest;
}
