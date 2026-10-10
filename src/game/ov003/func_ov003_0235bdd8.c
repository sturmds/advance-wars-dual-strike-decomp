#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit *Ov003Unit_SpawnNext(void);
void func_ov003_02364204(int x, int y);

int func_ov003_0235bdd8(void)
{
    Ov003Unit *unit = Ov003Unit_SpawnNext();

    if (unit == NULL) {
        return 0;
    }
    unit->unk_17_6 = 1;
    unit->unk_08 = 180;
    func_ov003_02364204(unit->x >> 3, unit->y >> 3);
    return 1;
}
