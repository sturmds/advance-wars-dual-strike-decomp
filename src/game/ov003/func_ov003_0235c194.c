#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit *Ov003Unit_Create(int slot, int column, int x, int y);
void Ov003_CreateUnitSprite(int x, int y);

BOOL func_ov003_0235c194(int slot, int column, int x, int y)
{
    Ov003Unit *unit = Ov003Unit_Create(slot, column, x, y);
    BOOL ok = FALSE;
    if (unit != NULL) {
        unit->unk_17_6 = 1;
        unit->safeTimer = 180;
        Ov003_CreateUnitSprite(unit->x >> 3, unit->y >> 3);
        ok = TRUE;
    }
    return ok;
}
