#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[16];

/* Copies x, y, unk_04 and hp of each live unit into its start fields. It runs
 * every frame (from func_ov003_02358658), so they hold the previous frame's
 * values, not the values at creation. */
void Ov003_SnapshotUnits(void)
{
    Ov003Unit *unit = data_ov003_02369538;
    int i;

    for (i = 0; i < 16; i++, unit++) {
        if (unit->hp != 0) {
            unit->startX = unit->x;
            unit->unk_0c = unit->y;
            unit->startUnk_04 = unit->unk_04;
            unit->startHp = unit->hp;
        }
    }
}
