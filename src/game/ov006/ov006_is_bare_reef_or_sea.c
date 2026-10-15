#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* 1 if the tile at (x, y) is unowned reef or sea, else 0. */
int Ov006_IsBareReefOrSea(int x, int y) {
  u8 tile = MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]];

  if (tile == TERRAIN_REEF || tile == TERRAIN_SEA) {
    return 1;
  }
  return 0;
}
