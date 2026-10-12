#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define TERRAIN_BIT(t) (1 << ((t) - TERRAIN_SEA))
#define SEA_MASK                                           \
  (TERRAIN_BIT(TERRAIN_SEA) | TERRAIN_BIT(TERRAIN_SHOAL) | \
   TERRAIN_BIT(TERRAIN_REEF))

/* 1 if the tile at x, y is not sea, shoal or reef, else 0. */
int Ov006_IsNotSeaTile(int x, int y) {
  MapTiles* map = data_027c027c->unk_48[data_027c0284]->mapTiles;
  int isSea = 0;
  u32 offset = map->tiles[map->rowStart[y] + x] - TERRAIN_SEA;

  if (offset <= TERRAIN_REEF - TERRAIN_SEA && ((1 << offset) & SEA_MASK)) {
    isSea = 1;
  }
  return !isSea;
}
