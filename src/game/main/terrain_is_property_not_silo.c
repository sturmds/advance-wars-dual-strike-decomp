#include "types.h"
#include "game/terrain.h"

extern u8 gTerrainIsProperty[32];

/* TRUE for a property terrain other than a missile silo. */
BOOL Terrain_IsPropertyNotSilo(int terrain) {
  int id = terrain & 31;

  if (id == TERRAIN_SILO) {
    return FALSE;
  }
  return gTerrainIsProperty[id] != 0;
}
