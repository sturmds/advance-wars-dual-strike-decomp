#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02160ac0.h"
#include "game/math.h"

BOOL CombatModeMenu_IsSlotAvailable(u8 slot);
void func_0208ad2c(void);

extern MenuButtonTemplate gCombatModeSlotButton;

/* Rebuilds the buttons of the Combat mode menu: one per available slot, 3 per
 * row, then the extra buttons added by func_0208ad2c. The button of the saved
 * slot is selected before the others exist. */
void CombatModeMenu_AddButtons(void) {
  int x = 48;
  int y = 64;
  int i;

  MenuButtons_RemoveSelectable();
  MenuButton_Select(data_02160ac0->co);
  for (i = 0; i < COMBAT_CO_COUNT; i++) {
    if (CombatModeMenu_IsSlotAvailable(i) == TRUE) {
      MenuButton_Add(x, y, &gCombatModeSlotButton, i);
    }
    x += 56;
    if (HwMod32(i, 3) == 2) {
      x -= 168;
      y += 24;
    }
  }
  func_0208ad2c();
}
