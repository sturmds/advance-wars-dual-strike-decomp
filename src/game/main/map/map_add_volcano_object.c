#include "types.h"
#include "game/map_object.h"

void MapObject_GetKindOffset(int kind, s16* offset);
void Map_AddObject(int x, int y, int w, int h, int kind, int hp, int state,
                   int startState, int variant, int unk07);

/* Places a volcano object. hp 0 does not make it count as destroyed (MapObject_IsStanding). (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddVolcanoObject(int x, int y, int w, int h, int state, int startState,
                          int variant) {
  s16 offset[2];

  MapObject_GetKindOffset(MAP_OBJECT_VOLCANO, offset);
  Map_AddObject(x - offset[0], y - offset[1], w, h, MAP_OBJECT_VOLCANO, 0,
                state, startState, variant, 50);
}
