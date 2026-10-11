#include "types.h"
#include "game/map_object.h"

void func_020db054(int kind, s16 *offset);
void func_020dae80(int x, int y, int w, int h, int kind, int hp, int state, int startState, int variant, int unk07);

/* Places a 3 by 3 Black Obelisk object. (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddBlackObeliskObject(int x, int y)
{
    s16 offset[2];

    func_020db054(MAP_OBJECT_BLACK_OBELISK, offset);
    func_020dae80(x - offset[0], y - offset[1], 3, 3, MAP_OBJECT_BLACK_OBELISK, 99, 0, 0, 0, 0);
}
