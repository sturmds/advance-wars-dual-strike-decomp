#include "types.h"
#include "game/map_object.h"

MapObject* Map_FindObjectCovering(
    int x, int y); /* object whose area covers (x, y); NULL: none */
BOOL MapObject_GetTargetTile(
    MapObject* object,
    s16* pos); /* the tile units attack the object on; FALSE: kind has none */

/* Returns the object that units attack at (x, y), or NULL when the tile is
 * not an object's target tile or the object is destroyed. */
MapObject* Map_GetObjectAt(int x, int y) {
  s16 pos[2];
  MapObject* object = Map_FindObjectCovering(x, y);

  if (object == NULL) {
    return NULL;
  }
  if (!MapObject_GetTargetTile(object, pos)) {
    return NULL;
  }
  if (pos[0] != x || pos[1] != y) {
    return NULL;
  }
  if (object->hp == 0) {
    object = NULL;
  }
  return object;
}
