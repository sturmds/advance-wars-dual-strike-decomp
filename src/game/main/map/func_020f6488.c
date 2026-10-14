#include "types.h"
#include "game/map_bg.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16 gOwnerCellCodes[];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define CELL(x, y) (GAME->mapTiles->mapCells[(x) + GAME->mapTiles->rowStart[y]])

/* gOwnerCellCodes lists map cell codes in rows of 5, one row per kind of
 * owned tile, one column per owner (tile bits 5-7), ending with 0xffff.
 * If the cell at (x, y) is in row 0, it becomes the row 2 cell for the
 * owner in tile. Returns the index of the row's first entry (35 when the
 * cell is not in the table). */
s16 func_020f6488(int x, int y, int tile) {
  int i;

  for (i = 0; gOwnerCellCodes[i] != OWNER_CELL_END; i++) {
    if (gOwnerCellCodes[i] == CELL(x, y)) {
      break;
    }
  }
  i = i / 5 * 5;
  if (i < 5) {
    CELL(x, y) = gOwnerCellCodes[i + 10 + (tile >> 5)];
  }
  return i;
}
