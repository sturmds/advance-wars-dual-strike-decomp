#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* 1 if the tile at (x, y) is unowned road or bridge, else 0. */
int Ov006_IsRoadOrBridgeTile(int x, int y) {
  u8 tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];

  if (tile == TERRAIN_ROAD || tile == TERRAIN_BRIDGE) {
    return 1;
  }
  return 0;
}
