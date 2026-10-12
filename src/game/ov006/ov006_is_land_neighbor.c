#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_IsSpecialCell(int x, int y);
int Ov006_IsBridgeTile(int x, int y);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define TERRAIN_BIT(t) (1 << ((t) - TERRAIN_SEA))
#define SEA_MASK                                           \
  (TERRAIN_BIT(TERRAIN_SEA) | TERRAIN_BIT(TERRAIN_SHOAL) | \
   TERRAIN_BIT(TERRAIN_REEF))

/* 1 if tile x, y passes none of the three checks: sea, shoal or reef terrain,
 * Ov006_IsBridgeTile, and (on a river tile) Ov006_IsSpecialCell. Ov006_GetNeighborMask
 * builds its mask from this. Names of the checks are guesses. */
int Ov006_IsLandNeighbor(int x, int y) {
  int otherCheck;
  int blocked;
  int seaOrOther;
  u32 offset;
  int riverCheck;
  MapTiles* map;
  int terrain;
  int isSea;
  map = data_027c027c->unk_48[data_027c0284]->mapTiles;
  terrain = map->tiles[map->rowStart[y] + x];

  if (terrain != TERRAIN_RIVER) {
    riverCheck = 0;
  } else {
    riverCheck = Ov006_IsSpecialCell(x, y);
  }
  otherCheck = Ov006_IsBridgeTile(x, y);
  blocked = 1;
  seaOrOther = blocked;
  isSea = 0;
  offset = terrain - TERRAIN_SEA;
  if (offset <= TERRAIN_REEF - TERRAIN_SEA && ((1 << offset) & SEA_MASK)) {
    isSea = 1;
  }
  if (!isSea && otherCheck == 0) {
    seaOrOther = 0;
  }
  if (!seaOrOther && riverCheck == 0) {
    blocked = 0;
  }
  return !blocked;
}
