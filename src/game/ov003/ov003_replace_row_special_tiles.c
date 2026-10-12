#include "types.h"
#include "game/ov003_unit.h"

extern u8
    data_ov003_02365d10[]; /* per tile code: bits 5-7 row, bits 0-4 terrain */

#define MAP data_ov003_02364a88
#define CELL(x, y) (MAP->cells[(y) * MAP->width + (x)])

/* Swaps the tile code of every tile that belongs to row (1 to 5) and has
 * terrain 6, 8, 10, 11, 14, 20 or 22 for a fixed code per terrain (0x1b9 to
 * 0x1d9). Ov003_LoadMap calls it for rows that are not active. */
void Ov003_ReplaceRowSpecialTiles(int row) {
  int x;
  int y;

  for (y = 0; y < MAP->height; y++) {
    for (x = 0; x < MAP->width; x++) {
      u8 tile = data_ov003_02365d10[CELL(x, y)];

      if (((tile >> 5) & 7) != row) {
        continue;
      }
      switch (tile & 31) {
        case 6:
        case 8:
          CELL(x, y) = 0x1c2;
          break;
        case 14:
          CELL(x, y) = 0x1c1;
          break;
        case 10:
          CELL(x, y) = 0x1c3;
          break;
        case 11:
          CELL(x, y) = 0x1c4;
          break;
        case 22:
          CELL(x, y) = 0x1b9;
          break;
        case 20:
          CELL(x, y) = 0x1d9;
          break;
      }
    }
  }
}
