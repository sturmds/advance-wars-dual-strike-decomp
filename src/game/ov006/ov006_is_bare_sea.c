#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* 1 if the tile at (x, y) is unowned sea, else 0. The reef test is redundant
 * but needed to match. */
int Ov006_IsBareSea(int x, int y) {
  u8 tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];

  if (tile == TERRAIN_REEF || tile != TERRAIN_SEA) {
    return 0;
  }
  return 1;
}
