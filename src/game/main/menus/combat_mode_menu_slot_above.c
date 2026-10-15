#include "types.h"
#include "game/math.h"

BOOL CombatModeMenu_IsSlotAvailable(u8 slot);

/* Moves up one row of the 3-wide slot grid, wrapping from the first row to
 * the last, and passes over slots that are not available. */
int CombatModeMenu_SlotAbove(int slot) {
  int rows = HwDiv32(6, 3) + 1;
  int col = HwMod32(slot, 3);
  int lastRowStep = (rows - 1) * 3;

  do {
    if (slot == col) {
      slot += lastRowStep;
    } else {
      slot -= 3;
    }
  } while (!CombatModeMenu_IsSlotAvailable(slot));
  return slot;
}
