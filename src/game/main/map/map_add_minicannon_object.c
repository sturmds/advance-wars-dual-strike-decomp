#include "types.h"
#include "game/map_object.h"

void MapObject_GetKindOffset(int kind, s16* offset);
void Map_AddObject(int x, int y, int w, int h, int kind, int hp, int state,
                   int startState, int variant, int unk07);

/* Places a Minicannon object; the variant is the tile owner. (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddMinicannonObject(int x, int y, int state, int startState,
                             int owner) {
  s16 offset[2];

  MapObject_GetKindOffset(MAP_OBJECT_MINICANNON, offset);
  Map_AddObject(x - offset[0], y - offset[1], 1, 1, MAP_OBJECT_MINICANNON, 99,
                state, startState, owner, 30);
}
