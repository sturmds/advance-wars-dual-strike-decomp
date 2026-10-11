#include "types.h"
#include "game/map_object.h"

void MapObject_GetKindOffset(int kind, s16 *offset);
void Map_AddObject(int x, int y, int w, int h, int kind, int hp, int state, int startState, int variant, int unk07);

/* Places a kind 5 object (terrain 30) with its top left tile at (x, y). (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddKind5Object(int x, int y, int w, int h, int state, int startState)
{
    s16 offset[2];

    MapObject_GetKindOffset(MAP_OBJECT_KIND_5, offset);
    Map_AddObject(x - offset[0], y - offset[1], w, h, MAP_OBJECT_KIND_5, 99, state, startState, 0, 80);
}
