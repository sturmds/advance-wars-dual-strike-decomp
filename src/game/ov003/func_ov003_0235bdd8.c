#include "types.h"
#include "game/ov003_unit.h"

Ov003Unit *Ov003Unit_SpawnNext(void);
void Ov003_CreateUnitSprite(int x, int y);

int func_ov003_0235bdd8(void)
{
    Ov003Unit *unit = Ov003Unit_SpawnNext();

    if (unit == NULL) {
        return 0;
    }
    unit->unk_17_6 = 1;
    unit->safeTimer = 180;
    Ov003_CreateUnitSprite(unit->x >> 3, unit->y >> 3);
    return 1;
}
