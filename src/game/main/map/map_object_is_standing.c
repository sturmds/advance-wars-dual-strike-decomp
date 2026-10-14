#include "types.h"
#include "game/map_object.h"

/* The objects that can be destroyed stand while their hp is not 0; the
 * volcano and kinds 6 to 8 always stand; an empty list entry does not. */
BOOL MapObject_IsStanding(MapObject* object) {
  switch (object->kind) {
    case MAP_OBJECT_LASER:
    case MAP_OBJECT_BLACK_CANNON:
    case MAP_OBJECT_MINICANNON:
    case MAP_OBJECT_DEATHRAY:
    case MAP_OBJECT_BLACK_CRYSTAL:
    case MAP_OBJECT_BLACK_OBELISK:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 1:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 2:
      return object->hp != 0;
    case MAP_OBJECT_NONE:
      return FALSE;
    default:
      return TRUE;
  }
}
