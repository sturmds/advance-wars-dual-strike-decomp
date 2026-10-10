#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8d[][20];
extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireForward(Ov003Unit *unit);

/* One shot of a burst straight ahead. unk_1d_0 counts the shots; when it
 * reaches the per-tile burst length it restarts and unk_19 gets the wait
 * before the next burst. */
void Ov003Unit_FireForwardBurst(Ov003Unit *unit)
{
    Ov003Unit_FireForward(unit);
    unit->unk_1d_0++;
    if (unit->unk_1d_0 != data_ov003_02364d8d[unit->tile][0]) {
        return;
    }
    unit->unk_1d_0 = 0;
    unit->unk_19 = data_ov003_02364d8e[unit->tile][0];
}
