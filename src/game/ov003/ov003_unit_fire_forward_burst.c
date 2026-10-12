#include "types.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02364d8d[][20];
extern u8 data_ov003_02364d8e[][20];

void Ov003Unit_FireForward(Ov003Unit* unit);

/* One shot of a burst straight ahead. shotCount counts the shots; when it
 * reaches the per-tile burst length it restarts and shotWait gets the wait
 * before the next burst. */
void Ov003Unit_FireForwardBurst(Ov003Unit* unit) {
  Ov003Unit_FireForward(unit);
  unit->shotCount++;
  if (unit->shotCount != data_ov003_02364d8d[unit->kind][0]) {
    return;
  }
  unit->shotCount = 0;
  unit->shotWait = data_ov003_02364d8e[unit->kind][0];
}
