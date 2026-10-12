#include "types.h"

/* Packs 12 values of 0-15 into 6 bytes, high nibble first. */
void PackNibblePairs(const u8* nibbles, u8* out) {
  int i;

  for (i = 0; i < 6; i++) {
    out[i] = ((nibbles[i * 2] & 15) << 4) | (nibbles[i * 2 + 1] & 15);
  }
}
