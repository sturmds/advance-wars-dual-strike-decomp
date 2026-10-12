#include "types.h"

/* Undoes PackNibblePairs: 6 bytes to 12 values, high nibble first. A nibble of 15 becomes 0xff. */
void UnpackNibblePairs(const u8* packed, u8* nibbles) {
  int i = 0;
  int hi = 0;
  int lo = 1;

  do {
    u8 n = (packed[i] >> 4) & 0xf;
    const u8* p = packed + i;
    i++;
    if (n == 0xf) {
      nibbles[hi] = 0xff;
    } else {
      nibbles[hi] = n;
    }
    hi += 2;
    n = *p & 0xf;
    if (n == 0xf) {
      nibbles[lo] = 0xff;
    } else {
      nibbles[lo] = n;
    }
    lo += 2;
  } while (i < 6);
}
