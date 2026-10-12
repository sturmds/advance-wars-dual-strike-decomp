#include "types.h"
#include "game/map_object.h"

void MapObject_GetKindOffset(int kind, s16* offset);
void Map_AddObject(int x, int y, int w, int h, int kind, int hp, int state,
                   int startState, int variant, int unk07);

/* Places a 1 by 1 Black Crystal object. (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddBlackCrystalObject(int x, int y) {
  s16 offset[2];

  MapObject_GetKindOffset(MAP_OBJECT_BLACK_CRYSTAL, offset);
  Map_AddObject(x - offset[0], y - offset[1], 1, 1, MAP_OBJECT_BLACK_CRYSTAL,
                99, 0, 0, 0, 0);
}
