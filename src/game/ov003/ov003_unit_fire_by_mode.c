#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8d[][20];
extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireForward(Ov003Unit* unit);
void Ov003Unit_FireAtUnitScattered(Ov003Unit* unit, int targetIndex);

#define STATE data_ov003_02364928

/* One shot of a burst. Mode 0 (Ov003State.unk_1df_2) fires straight ahead, mode 1
 * does too for tile 4 and 5, and every other case aims at the unit in target
 * with a random miss. shotCount counts the shots; at the per-tile burst length it
 * restarts and shotWait gets the wait before the next burst. */
void Ov003Unit_FireByMode(Ov003Unit* unit) {
  switch (STATE->unk_1df_2) {
    case 0:
      Ov003Unit_FireForward(unit);
      break;
    case 1:
      switch (unit->kind) {
        case 4:
        case 5:
          Ov003Unit_FireForward(unit);
          break;
        default:
          Ov003Unit_FireAtUnitScattered(unit, unit->target);
          break;
      }
      break;
    default:
      Ov003Unit_FireAtUnitScattered(unit, unit->target);
      break;
  }
  unit->shotCount++;
  if (unit->shotCount != data_ov003_02364d8d[unit->kind][0]) {
    return;
  }
  unit->shotCount = 0;
  unit->shotWait = data_ov003_02364d8e[unit->kind][0];
}
