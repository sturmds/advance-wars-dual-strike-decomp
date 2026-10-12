#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8d[][20];
extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireAtUnitScattered(Ov003Unit* unit, int targetIndex);

/* One shot of a burst at the unit in scatterTarget, aimed with a random miss. The
 * target (scatterTarget) clears when the burst ends. */
void Ov003Unit_FireScatteredBurst(Ov003Unit* unit) {
  Ov003Unit_FireAtUnitScattered(unit, unit->scatterTarget);
  unit->shotCount++;
  if (unit->shotCount != data_ov003_02364d8d[unit->kind][0]) {
    return;
  }
  unit->shotCount = 0;
  unit->scatterTarget = -1;
  unit->shotWait = data_ov003_02364d8e[unit->kind][0];
}
