#include "types.h"

/* Writes count tile pairs into two rows of a 32-wide BG screen map: entry i
 * of the top row is tile + 2 * i and the entry below it is tile + 2 * i + 1.
 * The glyph tiles are 8x16, stored as top and bottom tile one after
 * the other. */
void Text_PutGlyphTiles(u16* map, int tile, int count) {
  int i;
  int bottom;
  int first;

  i = 0;
  if (i < count) {
    first = tile;
    bottom = 1;
    do {
      map[i] = tile;
      (&map[i])[32] = first + bottom;
      tile += 2;
      bottom += 2;
      i++;
    } while (i < count);
  }
}
