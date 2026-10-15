#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_IsSpecialCell(int x, int y);

/* 1 if the tile at (x, y) is unowned river and Ov006_IsSpecialCell is 0. */
int Ov006_IsPlainRiver(int x, int y) {
  if (MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] ==
      TERRAIN_RIVER) {
    return !Ov006_IsSpecialCell(x, y);
  }
  return 0;
}
