#include "types.h"
#include "game/Unk_02088d04.h"

/* Writes two tiles side by side into the 32-tile-wide BG screen at
 * layer0Map: tiles firstTile and firstTile + 1 at (x, y), with palette. */
void Bg_PutTwoTiles(int x, int y, int firstTile, int palette) {
  int pos = x + y * 32;

  data_027c0280->layer0Map[pos] = firstTile | (palette << 12);
  data_027c0280->layer0Map[pos + 1] = (firstTile + 1) | (palette << 12);
}
