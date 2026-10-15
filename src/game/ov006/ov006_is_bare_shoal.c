#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

/* 1 if the tile at (x, y) is unowned shoal, else 0. */
int Ov006_IsBareShoal(int x, int y) {
  return MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] ==
         TERRAIN_SHOAL;
}
