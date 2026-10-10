#include "types.h"
#include "game/ov003_object.h"
#include "game/ov003_unit.h"

Ov003Unit *Ov003_FindUnitNear(int x, int y, int skipUnit);
void func_ov003_0235b64c(Ov003Object *object, Ov003Unit *unit);
int func_ov003_0235b8ac(int x, int y, Ov003Object *object);
void func_ov003_0235aff4(Ov003Object *object, int tileX, int tileY);

/* Tests the point (x, y), in 8-pixel units, for object. A unit near it (not
 * the unit named by object->unk_0a) goes to func_ov003_0235b64c; else a hit
 * found by func_ov003_0235b8ac goes to func_ov003_0235aff4 with the 16-pixel
 * tile. Returns 1 for either hit. */
int Ov003Object_HitAt(Ov003Object *object, int x, int y)
{
    Ov003Unit *unit = Ov003_FindUnitNear(x, y, object->unk_0a);

    if (unit != NULL) {
        func_ov003_0235b64c(object, unit);
        return 1;
    }
    if (func_ov003_0235b8ac(x, y, object) != 1) {
        return 0;
    }
    func_ov003_0235aff4(object, x >> 4, y >> 4);
    return 1;
}
