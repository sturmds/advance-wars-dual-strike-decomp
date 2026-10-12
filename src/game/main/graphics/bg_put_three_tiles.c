#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Writes three tiles side by side into the 32-tile-wide BG screen at
 * layer0Map: tiles firstTile to firstTile + 2 at (x, y), with palette. */
void Bg_PutThreeTiles(int x, int y, int firstTile, int palette) {
  int pos = x + y * 32;

  data_027c0280->layer0Map[pos] = firstTile | (palette << 12);
  data_027c0280->layer0Map[pos + 1] = (firstTile + 1) | (palette << 12);
  data_027c0280->layer0Map[pos + 2] = (firstTile + 2) | (palette << 12);
}
