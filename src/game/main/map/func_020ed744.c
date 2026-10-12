#include "types.h"
#include "game/map_object.h"

BOOL MapObject_GetTargetTile(MapObject* object, s16* pos);
void func_020eed64(int x, int y, int arg);

/* Calls func_020eed64 with the object's target tile. */
void func_020ed744(MapObject* object, int arg) {
  s16 pos[2];

  MapObject_GetTargetTile(object, pos);
  func_020eed64(pos[0], pos[1], arg);
}
