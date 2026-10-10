#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit *Ov003Unit_Create(int slot, int column, int x, int y);
void func_ov003_02364204(int x, int y);

BOOL func_ov003_0235c194(int slot, int column, int x, int y)
{
    Ov003Unit *unit = Ov003Unit_Create(slot, column, x, y);
    BOOL ok = FALSE;
    if (unit != NULL) {
        unit->unk_17_6 = 1;
        unit->unk_08 = 180;
        func_ov003_02364204(unit->x >> 3, unit->y >> 3);
        ok = TRUE;
    }
    return ok;
}
