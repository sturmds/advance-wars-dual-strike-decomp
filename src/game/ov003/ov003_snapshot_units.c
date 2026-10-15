#include "types.h"
#include "game/ov003_unit.h"

/* Copies x, y, unk_04 and hp of each live unit into its start fields. It runs
 * every frame (from Ov003_TickUnits), so they hold the previous frame's
 * values, not the values at creation. */
void Ov003_SnapshotUnits(void) {
  Ov003Unit* unit = data_ov003_02369538;
  int i;

  for (i = 0; i < OV003_UNIT_COUNT; i++, unit++) {
    if (unit->hp != 0) {
      unit->prevX = unit->x;
      unit->prevY = unit->y;
      unit->prevUnk_04 = unit->unk_04;
      unit->prevHp = unit->hp;
    }
  }
}
