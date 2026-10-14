#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_item.h"

void Sound_PlaySe(u32 soundId);
void Ov003Row_AddCharge(int a0, int a1);
void Ov003_DamageUnitsAround(int a0, int a1, int a2);
void Ov003_CreateSpriteVariant0(int a0, int a1);

/* Gives the unit that touched the pickup its effect, by kind: 1 adds 20 hp
 * (to 100 at most), 2 sets unit->safeTimer to 360, 3 and 4 call routines not
 * understood yet with the unit's row (4 also with the pickup's position), 5
 * sets Ov003Row.slowTimer of every other row to 420. Then the pickup is freed. */
void Ov003_ApplyPickup(Ov003Unit* unit, Ov003Item* pickup) {
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
      unit->safeTimer = 360;
      break;
    case 3:
      Ov003Row_AddCharge(unit->row, 100);
      break;
    case 4:
      Ov003_DamageUnitsAround(unit->row, pickup->x, pickup->y);
      Ov003_CreateSpriteVariant0(pickup->x >> 3, pickup->y >> 3);
      break;
    case 5:
      for (i = 1; i <= OV003_ROW_COUNT; i++) {
        if (i != unit->row) {
          data_ov003_02364928->rows[i - 1].slowTimer = 420;
        }
      }
      break;
  }
  pickup->timer = 0;
}
