#include "types.h"
#include "game/map_object.h"

BOOL MapObject_GetTargetTile(MapObject* object, s16* pos);
void func_020eeda0(int x, int y, int arg);

void func_020ed774(MapObject* object, int arg) {
  s16 pos[2];

  MapObject_GetTargetTile(object, pos);
  func_020eeda0(pos[0], pos[1], arg);
}
