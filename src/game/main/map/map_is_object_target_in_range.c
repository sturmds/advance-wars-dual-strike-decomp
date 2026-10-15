#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_object.h"

MapObject* Map_FindObjectCovering(int x, int y);
BOOL MapObject_GetTargetTile(MapObject* object, s16* pos);

/* TRUE when the object covering (x, y) has a target tile that the range map reaches (not negative). */
BOOL Map_IsObjectTargetInRange(int x, int y) {
  s16 pos[2];
  MapObject* object = Map_FindObjectCovering(x, y);

  if (object == NULL) {
    return FALSE;
  }
  if (!MapObject_GetTargetTile(object, pos)) {
    return FALSE;
  }
  return MAP_STATE->mapTiles
             ->rangeMapTiles[pos[0] + MAP_STATE->mapTiles->rowStart[pos[1]]] >=
         0;
}
