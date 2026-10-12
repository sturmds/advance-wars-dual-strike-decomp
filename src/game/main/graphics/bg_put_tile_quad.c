#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Writes tiles firstTile to firstTile + 3 as a 2x2 block at (x, y) of the
 * layer 0 tile map (32 tiles wide), with palette. */
void Bg_PutTileQuad(int x, int y, int firstTile, int palette) {
  int pos = x + y * 32;

  data_027c0280->layer0Map[pos] = firstTile | (palette << 12);
  data_027c0280->layer0Map[pos + 1] = (firstTile + 1) | (palette << 12);
  data_027c0280->layer0Map[pos + 32] = (firstTile + 2) | (palette << 12);
  data_027c0280->layer0Map[pos + 33] = (firstTile + 3) | (palette << 12);
}
