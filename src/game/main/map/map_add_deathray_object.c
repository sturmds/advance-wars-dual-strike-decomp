#include "types.h"
#include "game/map_object.h"

void MapObject_GetKindOffset(int kind, s16 *offset);
void Map_AddObject(int x, int y, int w, int h, int kind, int hp, int state, int startState, int variant, int unk07);

/* Places a Deathray object (terrain 30), 3x3. (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddDeathrayObject(int x, int y, int w, int h, int state, int startState)
{
    s16 offset[2];

    MapObject_GetKindOffset(MAP_OBJECT_DEATHRAY, offset);
    Map_AddObject(x - offset[0], y - offset[1], w, h, MAP_OBJECT_DEATHRAY, 99, state, startState, 0, 80);
}
