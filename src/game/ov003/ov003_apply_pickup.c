#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_pickup.h"

void Sound_PlaySe(u32 soundId);
void Ov003Row_AddGauge(int a0, int a1);
void func_ov003_0235e16c(int a0, int a1, int a2);
void func_ov003_02364618(int a0, int a1);

/* Gives the unit that touched the pickup its effect, by kind: 1 adds 20 hp
 * (to 100 at most), 2 sets unit->unk_08 to 360, 3 and 4 call routines not
 * understood yet with the unit's row (4 also with the pickup's position), 5
 * sets Ov003Row.unk_36 of every other row to 420. Then the pickup is freed. */
void Ov003_ApplyPickup(Ov003Unit *unit, Ov003Pickup *pickup)
{
    int i;

    Sound_PlaySe(244);
    switch (pickup->kind) {
    case 1:
        unit->hp += 20;
        if (unit->hp > 100) {
            unit->hp = 100;
        }
        break;
    case 2:
        unit->unk_08 = 360;
        break;
    case 3:
        Ov003Row_AddGauge(unit->unk_12_3, 100);
        break;
    case 4:
        func_ov003_0235e16c(unit->unk_12_3, pickup->x, pickup->y);
        func_ov003_02364618(pickup->x >> 3, pickup->y >> 3);
        break;
    case 5:
        for (i = 1; i <= 5; i++) {
            if (i != unit->unk_12_3) {
                data_ov003_02364928->rows[i - 1].unk_36 = 420;
            }
        }
        break;
    }
    pickup->time = 0;
}
