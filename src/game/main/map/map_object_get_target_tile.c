#include "types.h"
#include "game/map_object.h"

/* Writes the tile units attack the object on to pos: the object's own x and
 * y for kinds 1, 4, 9, 11, 12 and 13, one right and two down for kinds 3, 5
 * and 10. Returns FALSE for the other kinds. */
BOOL MapObject_GetTargetTile(MapObject* object, s16* pos) {
  switch (object->kind) {
    case 1:
    case 4:
    case 9:
    case 11:
    case 12:
    case 13:
      pos[0] = object->x;
      pos[1] = object->y;
      return TRUE;
    case 3:
    case 5:
    case 10:
      pos[0] = object->x + 1;
      pos[1] = object->y + 2;
      return TRUE;
    default:
      return FALSE;
  }
}
