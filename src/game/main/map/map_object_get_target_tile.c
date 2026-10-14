#include "types.h"
#include "game/map_object.h"

/* Writes the tile units attack the object on to pos: the object's own tile
 * for the 1x1 kinds, one right and two down (the bottom centre) for the 3x3
 * Black Cannon, Deathray and Black Obelisk. Returns FALSE for the other
 * kinds. */
BOOL MapObject_GetTargetTile(MapObject* object, s16* pos) {
  switch (object->kind) {
    case MAP_OBJECT_LASER:
    case MAP_OBJECT_MINICANNON:
    case MAP_OBJECT_BLACK_CRYSTAL:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 1:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 2:
      pos[0] = object->x;
      pos[1] = object->y;
      return TRUE;
    case MAP_OBJECT_BLACK_CANNON:
    case MAP_OBJECT_DEATHRAY:
    case MAP_OBJECT_BLACK_OBELISK:
      pos[0] = object->x + 1;
      pos[1] = object->y + 2;
      return TRUE;
    default:
      return FALSE;
  }
}
