#include "types.h"

int HwDiv32(int a, int b);
int HwMod32(int a, int b);
BOOL CombatModeMenu_IsSlotAvailable(u8 slot);

/* Moves down one row of the 3-wide slot grid, wrapping from the last row to
 * the first, and passes over slots that are not available. */
int CombatModeMenu_SlotBelow(int slot) {
  int rows = HwDiv32(6, 3) + 1;
  int col = HwMod32(slot, 3);
  int lastRowSlot = (rows - 1) * 3 + col;

  do {
    if (slot == lastRowSlot) {
      slot = col;
    } else {
      slot += 3;
    }
  } while (!CombatModeMenu_IsSlotAvailable(slot));
  return slot;
}
