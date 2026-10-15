#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

/* 1 if the tile at (x, y) is road, or a bridge whose cell is the horizontal
 * bridge piece; else 0. */
int Ov006_IsRoadOrHorizontalBridge(int x, int y) {
  int index = x + MAP_STATE->mapTiles->rowStart[y];
  u8 tile = MAP_STATE->mapTiles->tiles[index];
  u16 cell = MAP_STATE->mapTiles->mapCells[index];

  if (tile == TERRAIN_ROAD ||
      (tile == TERRAIN_BRIDGE && cell == MAP_CELL_BRIDGE_HORIZONTAL)) {
    return 1;
  }
  return 0;
}
