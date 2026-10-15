#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

/* Draws the move and attack range highlight for map tile (x, y): a tile with
 * a non-negative rangeMap value gets the four highlight characters, any other
 * is cleared. */
void MapView_DrawRangeTile(s16 x, s16 y) {
  u16* cell;

  /* Layer 0 holds a 16 by 16 tile window of the map, 2 by 2 characters per tile. */
  cell = data_027c0280->layer0Map +
         ((x - MAP_STATE->unk_1c4->unk_26) & 15) * 2 +
         ((y - MAP_STATE->unk_1c4->unk_28) & 15) * 64;
  if (MAP_STATE->rangeMap[y][x] < 0) {
    cell[0] = 0;
    cell[1] = 0;
    cell[32] = 0;
    cell[33] = 0;
  } else {
    cell[0] = 0x1d7;
    cell[1] = 0x1d8;
    cell[32] = 0x1d9;
    cell[33] = 0x1da;
  }
}
