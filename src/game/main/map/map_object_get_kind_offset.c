#include "types.h"
#include "game/map_object.h"

/* Writes the (x, y) tile offset the Map_Add*Object functions subtract from the position they are given. 0 for other kinds. */
void MapObject_GetKindOffset(int kind, s16* offset) {
  switch (kind) {
    case MAP_OBJECT_BLACK_CANNON:
    case MAP_OBJECT_DEATHRAY:
    case MAP_OBJECT_BLACK_OBELISK:
      offset[0] = 1;
      offset[1] = 1;
      break;
    case MAP_OBJECT_VOLCANO:
    case MAP_OBJECT_BLOCKED:
      offset[0] = 1;
      offset[1] = 2;
      break;
    default:
      offset[0] = 0;
      offset[1] = 0;
      break;
  }
}
