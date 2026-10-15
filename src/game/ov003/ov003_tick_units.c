#include "types.h"
#include "game/ov003_unit.h"

void Ov003_UpdateUnitTerrain(void);
void Ov003_SnapshotUnits(void);

/* Per frame: updates the units (Ov003_UpdateUnitTerrain), snapshots them, then
 * counts down unk_15, safeTimer and unk_11 of each live unit that has unk_17_6
 * clear, stopping each at 0. */
void Ov003_TickUnits(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  Ov003_UpdateUnitTerrain();
  Ov003_SnapshotUnits();
  for (i = 0; i < OV003_UNIT_COUNT; i++, unit++) {
    if (unit->hp != 0 && !unit->unk_17_6) {
      if (unit->unk_15 != 0) {
        unit->unk_15--;
      }
      if (unit->safeTimer != 0) {
        unit->safeTimer--;
      }
      if (unit->unk_11 != 0) {
        unit->unk_11--;
      }
    }
  }
}
