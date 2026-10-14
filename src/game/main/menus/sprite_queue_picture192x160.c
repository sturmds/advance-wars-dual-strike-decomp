#include "types.h"
#include "game/oam_cells.h"

void Sprite_QueueAt(int list, int x, int y, void* shape, int attr, int tile);

extern OamCellList1 data_027c0008;
extern u8 data_027c0030;

/* Queues one piece of the picture when any of it can show on the screen. */
#define PIECE(px, py, minY, shape, tileOffset)                      \
  if ((px) > -64 && (px) < 256 && (py) > (minY) && (py) < 192) {    \
    Sprite_QueueAt(list, (px) & 0x1ff, (py) & 0xff, &(shape), attr, \
                   tile + (tileOffset));                            \
  }

/* Draws a 3 by 3 block of sprites around x, y: two rows of 64 by 64 pieces
 * and one of 64 by 32, 192 by 160 pixels in all. tile is the first tile. */
void Sprite_QueuePicture192x160(s16 x, int y, int attr, int tile, int list) {
  PIECE(x - 96, y - 80, -64, data_027c0030, 0)
  PIECE(x - 32, y - 80, -64, data_027c0030, 64)
  PIECE(x + 32, y - 80, -64, data_027c0030, 128)
  PIECE(x - 96, y - 16, -64, data_027c0030, 192)
  PIECE(x - 32, y - 16, -64, data_027c0030, 256)
  PIECE(x + 32, y - 16, -64, data_027c0030, 320)
  PIECE(x - 96, y + 48, -32, data_027c0008, 384)
  PIECE(x - 32, y + 48, -32, data_027c0008, 416)
  PIECE(x + 32, y + 48, -32, data_027c0008, 448)
}
