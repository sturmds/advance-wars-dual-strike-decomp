#include "types.h"
#include "game/map_object.h"

void func_020db054(int kind, s16 *offset);
void func_020dae80(int x, int y, int w, int h, int kind, int hp, int state, int startState, int variant, int unk07);

/* Places a volcano object. hp 0 does not make it count as destroyed (MapObject_IsStanding). (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddVolcanoObject(int x, int y, int w, int h, int state, int startState, int variant)
{
    s16 offset[2];

    func_020db054(MAP_OBJECT_VOLCANO, offset);
    func_020dae80(x - offset[0], y - offset[1], w, h, MAP_OBJECT_VOLCANO, 0, state, startState, variant, 50);
}
