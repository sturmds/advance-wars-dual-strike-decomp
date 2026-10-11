#include "types.h"
#include "game/map_object.h"

void func_020db054(int kind, s16 *offset);
void func_020dae80(int x, int y, int w, int h, int kind, int hp, int state, int startState, int variant, int unk07);

/* Places a Grand Bolt weak point object; the kind is 11 plus the tile owner. (x, y) is moved back by the
 * kind's tile offset first. */
void Map_AddGrandBoltWeakPointObject(int x, int y, int owner)
{
    s16 offset[2];

    func_020db054(MAP_OBJECT_GRAND_BOLT_WEAK_POINT, offset);
    func_020dae80(x - offset[0], y - offset[1], 1, 1, MAP_OBJECT_GRAND_BOLT_WEAK_POINT + owner, 99, 0, 0, 0, 0);
}
