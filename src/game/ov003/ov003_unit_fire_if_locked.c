#include "types.h"
#include "game/ov003_unit.h"

void Ov003Unit_FireScatteredBurst(Ov003Unit* unit);
void Ov003Unit_FireAtLockedTarget(Ov003Unit* unit);

#define STATE data_ov003_02364928

/* Like func_ov003_0235f81c without the shots straight ahead: fires only at the
 * unit in scatterTarget (mode 2 or higher) or the one in lockedTarget. */
void Ov003Unit_FireIfLocked(Ov003Unit* unit) {
  if (unit->unk_15 != 0) {
    return;
  }
  if (unit->scatterTarget != -1 && STATE->unk_1df_2 >= 2) {
    Ov003Unit_FireScatteredBurst(unit);
    return;
  }
  if (unit->unk_1c_0) {
    Ov003Unit_FireAtLockedTarget(unit);
  }
}
