#include "types.h"
#include "game/map_object.h"

/* y of the tile offset that a cannon's shot starts from; see
 * MapObject_GetShotOriginX. */
int MapObject_GetShotOriginY(int kind, int variant) {
  int* table;

  switch (kind) {
    case MAP_OBJECT_BLACK_CANNON:
    default:
      table = gBlackCannonShotOrigins;
      break;
    case MAP_OBJECT_MINICANNON:
      table = gMinicannonShotOrigins;
      break;
  }
  return table[variant * 2 + 1];
}
