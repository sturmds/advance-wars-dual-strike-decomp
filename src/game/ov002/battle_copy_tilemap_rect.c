#include "types.h"

/* Copies a width x height block of tilemap entries; rows are 32 entries apart.
 * Needed to match: pointer additions, not array indexing (82% with dst[row * 32 + col]). */
void Battle_CopyTilemapRect(const u16* src, u16* dst, int width, int height) {
  int col;
  int row;

  for (row = 0; row < height; row++) {
    for (col = 0; col < width; col++) {
      *(dst + row * 32 + col) = *(src + row * 32 + col);
    }
  }
}
