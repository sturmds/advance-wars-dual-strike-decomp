#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/math.h"

void func_020f5878(int x, int y);

/* Calls func_020f5878 on the tile of the first of the current player's Stealths
 * whose fuel is at most a third of the type's full tank. The cast makes the
 * compare unsigned, as in the original. */
void Ov001_ShowLowFuelStealth(void) {
  int slot;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    MapUnit* unit = &MAP_STATE->units[slot];

    if (unit->type != UNIT_NONE && unit->type == UNIT_STEALTH &&
        Ov000_gUnitStats[unit->type].fuel != 0 &&
        unit->fuel <= (u32)HwDiv32(Ov000_gUnitStats[unit->type].fuel, 3)) {
      func_020f5878(unit->x, unit->y);
      return;
    }
  }
}
