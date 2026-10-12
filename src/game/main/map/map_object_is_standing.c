#include "types.h"
#include "game/map_object.h"

/* Kinds 1, 3, 4, 5 and 9 to 13 stand while their hp is not 0; kinds 2 and 6 to
 * 8 always stand; an empty list entry (kind 0) does not. */
BOOL MapObject_IsStanding(MapObject* object) {
  switch (object->kind) {
    case 1:
    case 3:
    case 4:
    case 5:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
      return object->hp != 0;
    case 0:
      return FALSE;
    default:
      return TRUE;
  }
}
