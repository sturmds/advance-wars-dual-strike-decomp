#include "types.h"

extern u8 gTerrainIsProperty[];

BOOL Terrain_IsProperty(int index) {
  return gTerrainIsProperty[index & 31] != 0;
}
