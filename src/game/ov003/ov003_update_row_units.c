#include "types.h"
#include "game/ov003_unit.h"

extern Ov003Unit data_ov003_02369538[];

int Ov003_IsPowerReady(int row);
void func_ov003_0235ca50(int row);
int func_ov003_02362098(int row);
void Ov003Unit_UpdateAi(Ov003Unit* unit);

#define STATE data_ov003_02364928

/* Runs Ov003Unit_UpdateAi on every unit in use whose row is the current one
 * (func_ov003_02362098 returns 0 for it), unless unk_1db_7 is set. Before
 * that, func_ov003_0235ca50 runs when Ov003_IsPowerReady says 1, slot 0 is in
 * state 1 and unit 0 is in use. The ead0 callee is not decompiled yet. */
void Ov003_UpdateRowUnits(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  if (STATE->unk_1db_7) {
    return;
  }
  if (Ov003_IsPowerReady(STATE->row) == 1 && STATE->slots[0].state == 1 &&
      data_ov003_02369538[0].hp != 0) {
    func_ov003_0235ca50(STATE->row);
  }
  for (i = 0; i < OV003_UNIT_COUNT; i++, unit++) {
    if (unit->hp != 0 && !unit->unk_17_6 &&
        func_ov003_02362098(unit->row) != 1) {
      Ov003Unit_UpdateAi(unit);
    }
  }
}
