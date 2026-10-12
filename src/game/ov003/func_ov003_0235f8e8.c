#include "types.h"
#include "game/ov003_unit.h"

void Ov003Unit_FireScatteredBurst(Ov003Unit* unit);
void Ov003Unit_FireAtLockedTarget(Ov003Unit* unit);
void Ov003Unit_FireForwardOnce(Ov003Unit* unit);
void Ov003Unit_FireByMode(Ov003Unit* unit);
#define STATE data_ov003_02364928

void func_ov003_0235f8e8(Ov003Unit* unit) {
  if (unit->unk_15 != 0) {
    return;
  }
  if (unit->scatterTarget != -1 && STATE->unk_1df_2 >= 2) {
    Ov003Unit_FireScatteredBurst(unit);
    return;
  }
  if (unit->unk_1c_0) {
    Ov003Unit_FireAtLockedTarget(unit);
    return;
  }
  if (unit->unk_1c_1 && unit->unk_1d_7) {
    Ov003Unit_FireForwardOnce(unit);
    return;
  }
  if (unit->shotWait != 0) {
    return;
  }
  Ov003Unit_FireByMode(unit);
}
