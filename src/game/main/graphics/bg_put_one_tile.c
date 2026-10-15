#include "types.h"
#include "game/Unk_02088d04.h"

/* Writes tile map entry tile with palette slot palette at (x, y) of the
 * layer 0 tile map buffer (32 entries per row). */
void Bg_PutOneTile(int x, int y, int tile, int palette) {
  data_027c0280->layer0Map[x + y * 32] = tile | (palette << 12);
}
