#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireAtUnit(Ov003Unit* unit, int targetIndex);

/* Fires once at the unit named by lockedTarget, then restarts the shot count and
 * the wait like Ov003Unit_FireForwardOnce. */
void Ov003Unit_FireAtLockedTarget(Ov003Unit* unit) {
  Ov003Unit_FireAtUnit(unit, unit->lockedTarget);
  unit->shotCount = 0;
  unit->shotWait = data_ov003_02364d8e[unit->kind][0];
}
