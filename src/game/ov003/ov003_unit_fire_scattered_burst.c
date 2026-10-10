#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8d[][20];
extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireAtUnitScattered(Ov003Unit *unit, int targetIndex);

/* One shot of a burst at the unit in unk_1b, aimed with a random miss. The
 * target (unk_1b) clears when the burst ends. */
void Ov003Unit_FireScatteredBurst(Ov003Unit *unit)
{
    Ov003Unit_FireAtUnitScattered(unit, unit->unk_1b);
    unit->unk_1d_0++;
    if (unit->unk_1d_0 != data_ov003_02364d8d[unit->tile][0]) {
        return;
    }
    unit->unk_1d_0 = 0;
    unit->unk_1b = -1;
    unit->unk_19 = data_ov003_02364d8e[unit->tile][0];
}
