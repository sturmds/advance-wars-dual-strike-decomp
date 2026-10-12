#include "types.h"
#include "game/ov005_ripple.h"

/* Keeps the whole-pixel part of each fx32 offset. */
void Ov005Ripple_PackOffsets(int* offsets, s16* packed) {
  int i;

  for (i = 0; i < OV005_RIPPLE_LINES; i++) {
    packed[i] = offsets[i] >> 12;
  }
}
