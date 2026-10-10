#include "types.h"
#include "game/ov003_object.h"
#include "game/ov003_unit.h"
#include "game/Unk_02160abc.h"

extern u8 data_ov003_02364d99[][20];
extern u8 data_ov003_02364d9a[][20];

s32 HwDiv32(s32 numerator, s32 denominator);
int Ov003_RandNext(void);
int Ov003_RollValue(int a, int b, int c, int d);
int func_ov003_0235d468(int row);
void Ov003Row_AddGauge(int a, int b);
void Ov003_CreateSpriteWithCallback(int x, int y, Ov003Unit *unit);
void Ov003_CreateSpriteVariant2(int x, int y);

/* Kill counts of an army's row, one byte per unit tile code. They start at
 * Ov003Row.unk_28 and run past that array into unk_32 and the bytes after. */
#define UNIT_COUNTS(army) (data_ov003_02364928->rows[(army) - 1].unk_28)

/* An object (a shot, kinds 0 to 7) hits a unit of another army. The damage
 * is a base value plus a random -3 to 4. A hit that would drop the unit's hp
 * to 0 kills it (unless the 02160abc flag is set), calls Ov003Row_AddGauge
 * for both armies and counts the kill for the attacker's army, with tile 2
 * counted as tile 1. Any other hit takes off hp and starts the unit's hurt
 * timer of 15. The object is spent either way. */
void Ov003Object_HitUnit(Ov003Object *object, Ov003Unit *unit)
{
    int damage = Ov003_RollValue(object->variant, object->kind, unit->unk_12_3, unit->tile);

    object->active = 0;
    if (object->variant == unit->unk_12_3) {
        return;
    }
    if (OV003_PHASE(data_ov003_02364928)->current == 7) {
        return;
    }
    damage += (Ov003_RandNext() & 7) - 3;
    if ((u16)unit->unk_08 == 0 && func_ov003_0235d468(unit->unk_12_3) == 0) {
        if (unit->hp <= damage && data_02160abc->unk_0a == 0) {
            unit->hp = 0;
            unit->unk_17_7 = 1;
            if (object->kind < 6) {
                Ov003Row_AddGauge(object->variant, data_ov003_02364d9a[unit->tile][0]);
                Ov003Row_AddGauge(unit->unk_12_3, HwDiv32(data_ov003_02364d9a[unit->tile][0] * 2, 3));
                if (UNIT_COUNTS(object->variant)[unit->tile] != 255) {
                    if (unit->tile == 2) {
                        UNIT_COUNTS(object->variant)[1]++;
                    } else {
                        UNIT_COUNTS(object->variant)[unit->tile]++;
                    }
                }
            }
            Ov003_CreateSpriteWithCallback(unit->x >> 3, unit->y >> 3, unit);
        } else {
            unit->hp -= damage;
            unit->unk_08 = 15;
            unit->unk_17_0 = 0;
            if (object->kind < 6) {
                Ov003Row_AddGauge(object->variant, data_ov003_02364d99[unit->tile][0]);
                Ov003Row_AddGauge(unit->unk_12_3, HwDiv32(data_ov003_02364d99[unit->tile][0] * 2, 3));
            }
            if (object->kind != 5 && object->kind != 7) {
                Ov003_CreateSpriteVariant2(object->x >> 3, object->y >> 3);
            }
        }
    }
    unit->unk_1b = object->unk_0a;
}
