#include "types.h"
#include "game/map_object.h"

/* x of the tile offset, from the object's top-left tile, that a cannon's
 * shot starts from, per facing (variant): the Minicannon's table for kind 4,
 * the Black Cannon's for every other kind. */
int MapObject_GetShotOriginX(int kind, int variant) {
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
  return table[variant * 2];
}
