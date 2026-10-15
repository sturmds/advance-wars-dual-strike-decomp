#include "types.h"
#include "game/map_bg.h"
#include "game/Unk_02080440.h"

extern u16 gOwnerCellCodes[];

#define CELL(x, y) \
  (MAP_STATE->mapTiles->mapCells[(x) + MAP_STATE->mapTiles->rowStart[y]])

/* Sets the map cell at (x, y) to the variant for the owner in tile (bits
 * 5-7), using the rows of 5 owner variants in gOwnerCellCodes (see
 * func_020f6488). Returns the index of the row's first entry. */
s16 MapCell_SetOwner(int x, int y, int tile) {
  int i;

  for (i = 0; gOwnerCellCodes[i] != OWNER_CELL_END; i++) {
    if (gOwnerCellCodes[i] == CELL(x, y)) {
      break;
    }
  }
  i = i / 5 * 5;
  CELL(x, y) = gOwnerCellCodes[i + (tile >> 5)];
  return i;
}
