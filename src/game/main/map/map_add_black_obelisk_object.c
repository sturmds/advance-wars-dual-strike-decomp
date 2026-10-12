#include "types.h"
#include "game/map_object.h"

void MapObject_GetKindOffset(int kind, s16* offset);
void Map_AddObject(int x, int y, int w, int h, int kind, int hp, int state,
                   int startState, int variant, int unk07);

/* Places a 3 by 3 Black Obelisk object. (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddBlackObeliskObject(int x, int y) {
  s16 offset[2];

  MapObject_GetKindOffset(MAP_OBJECT_BLACK_OBELISK, offset);
  Map_AddObject(x - offset[0], y - offset[1], 3, 3, MAP_OBJECT_BLACK_OBELISK,
                99, 0, 0, 0, 0);
}
