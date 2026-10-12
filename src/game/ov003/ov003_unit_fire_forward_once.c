#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireForward(Ov003Unit* unit);

/* Fires one shot in the unit's direction, restarts the shot count (shotCount)
 * and sets the wait before the next shot (shotWait) from the per-tile table. */
void Ov003Unit_FireForwardOnce(Ov003Unit* unit) {
  Ov003Unit_FireForward(unit);
  unit->shotCount = 0;
  unit->shotWait = data_ov003_02364d8e[unit->kind][0];
}
