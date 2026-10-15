#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

#define TERRAIN_BIT(t) (1 << ((t) - TERRAIN_RIVER))
#define WATER_MASK                                         \
  (TERRAIN_BIT(TERRAIN_RIVER) | TERRAIN_BIT(TERRAIN_SEA) | \
   TERRAIN_BIT(TERRAIN_SHOAL) | TERRAIN_BIT(TERRAIN_REEF))

/* 1 if the tile at x, y is not river, sea, shoal or reef, else 0. */
int Ov006_IsLandTile(int x, int y) {
  MapTiles* map = data_027c027c->unk_48[data_027c0284]->mapTiles;
  int isWater = 0;
  u32 offset = map->tiles[map->rowStart[y] + x] - TERRAIN_RIVER;

  if (offset <= TERRAIN_REEF - TERRAIN_RIVER && ((1 << offset) & WATER_MASK)) {
    isWater = 1;
  }
  return !isWater;
}
