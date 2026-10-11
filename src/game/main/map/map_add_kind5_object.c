#include "types.h"
#include "game/map_object.h"

void func_020db054(int kind, s16 *offset);
void func_020dae80(int x, int y, int w, int h, int kind, int hp, int state, int startState, int variant, int unk07);

/* Places a kind 5 object (terrain 30) with its top left tile at (x, y). (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddKind5Object(int x, int y, int w, int h, int state, int startState)
{
    s16 offset[2];

    func_020db054(MAP_OBJECT_KIND_5, offset);
    func_020dae80(x - offset[0], y - offset[1], w, h, MAP_OBJECT_KIND_5, 99, state, startState, 0, 80);
}
