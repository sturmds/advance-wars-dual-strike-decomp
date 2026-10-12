#include "types.h"

int HwDiv32(int a, int b);
int HwMod32(int a, int b);
BOOL CombatModeMenu_IsSlotAvailable(u8 slot);

extern u8* data_0229e144;

/* Moves left in the 3-wide slot grid, wrapping at the first column. Looks
 * for an available slot in the new column, starting at the remembered row
 * (data_0229e144) and going up, then down. */
int CombatModeMenu_SlotLeft(int slot) {
  int col = HwMod32(slot, 3);
  int row = HwDiv32(*data_0229e144, 3);
  int rows = HwDiv32(6, 3) + 1;
  int i;

  while (1) {
    if (col == 0) {
      col = 2;
    } else {
      col--;
    }
    for (i = row; i >= 0; i--) {
      if (CombatModeMenu_IsSlotAvailable(i * 3 + col) == 1) {
        return i * 3 + col;
      }
    }
    for (i = row; i < rows; i++) {
      if (CombatModeMenu_IsSlotAvailable(i * 3 + col) == 1) {
        return i * 3 + col;
      }
    }
  }
}
