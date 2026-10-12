#include "types.h"
#include "game/ov005_ripple.h"

void Ov005Ripple_FillOffsets(int* offsets, int value) {
  int i;

  for (i = 0; i < OV005_RIPPLE_LINES; i++) {
    offsets[i] = value;
  }
}
