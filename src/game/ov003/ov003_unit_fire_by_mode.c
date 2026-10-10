#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8d[][20];
extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireForward(Ov003Unit *unit);
void Ov003Unit_FireAtUnitScattered(Ov003Unit *unit, int targetIndex);

#define STATE data_ov003_02364928

/* One shot of a burst. Mode 0 (Ov003State.unk_1df_2) fires straight ahead, mode 1
 * does too for tile 4 and 5, and every other case aims at the unit in unk_1a
 * with a random miss. unk_1d_0 counts the shots; at the per-tile burst length it
 * restarts and unk_19 gets the wait before the next burst. */
void Ov003Unit_FireByMode(Ov003Unit *unit)
{
    switch (STATE->unk_1df_2) {
    case 0:
        Ov003Unit_FireForward(unit);
        break;
    case 1:
        switch (unit->tile) {
        case 4:
        case 5:
            Ov003Unit_FireForward(unit);
            break;
        default:
            Ov003Unit_FireAtUnitScattered(unit, unit->unk_1a);
            break;
        }
        break;
    default:
        Ov003Unit_FireAtUnitScattered(unit, unit->unk_1a);
        break;
    }
    unit->unk_1d_0++;
    if (unit->unk_1d_0 != data_ov003_02364d8d[unit->tile][0]) {
        return;
    }
    unit->unk_1d_0 = 0;
    unit->unk_19 = data_ov003_02364d8e[unit->tile][0];
}
