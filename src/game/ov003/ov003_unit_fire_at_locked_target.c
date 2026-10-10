#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireAtUnit(Ov003Unit *unit, int targetIndex);

/* Fires once at the unit named by unk_1c_2, then restarts the shot count and
 * the wait like Ov003Unit_FireForwardOnce. */
void Ov003Unit_FireAtLockedTarget(Ov003Unit *unit)
{
    Ov003Unit_FireAtUnit(unit, unit->unk_1c_2);
    unit->unk_1d_0 = 0;
    unit->unk_19 = data_ov003_02364d8e[unit->tile][0];
}
