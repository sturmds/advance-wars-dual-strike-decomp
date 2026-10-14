#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* 1 if the tile at (x, y) is road, or a bridge whose cell is the vertical
 * bridge piece; else 0. */
int Ov006_IsRoadOrVerticalBridge(int x, int y) {
  int index = x + STATE->mapTiles->rowStart[y];
  u8 tile = STATE->mapTiles->tiles[index];
  u16 cell = STATE->mapTiles->mapCells[index];

  if (tile == TERRAIN_ROAD ||
      (tile == TERRAIN_BRIDGE && cell == MAP_CELL_BRIDGE_VERTICAL)) {
    return 1;
  }
  return 0;
}
