#include "types.h"

/* Fills a width x height rectangle at (x, y) of a 32-tile-wide tile map with
 * consecutive tiles from firstTile, row by row, all in palette. */
void Bg_PutTileRect(u16* map, int x, int y, int width, u16 height,
                    u16 firstTile, u16 palette) {
  int row;
  int col;

  for (row = y; row < y + height; row++) {
    for (col = x; col < x + width; col++) {
      map[col + row * 32] = firstTile | (palette << 12);
      firstTile++;
    }
  }
}
