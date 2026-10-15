#include "types.h"
#include "game/Unk_02080440.h"

/* MAP_CELL_BRIDGE_HORIZONTAL or MAP_CELL_BRIDGE_VERTICAL. */
BOOL Ov006_IsBridgeCell(int x, int y) {
  u16 cell =
      MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]];

  return (u32)(cell - 21) <= 1;
}
