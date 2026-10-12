#include "types.h"

/* Sets count bytes at dest to 0xff. */
void Mem_FillBytesFF(u8* dest, int count) {
  while (count != 0) {
    *dest++ = 0xff;
    count--;
  }
}
