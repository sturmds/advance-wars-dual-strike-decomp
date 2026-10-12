#include "types.h"
#include "game/menu_button.h"

void CombatSetup_AddActionButtons(void);

extern MenuButtonTemplate data_021613f8;

/* Rebuilds the four Combat mode shop entries (buttons 8-11, 16 pixels
 * apart), the action buttons, and makes button 13 untouchable. */
void CombatModeMenu_AddShopButtons(void) {
  int y = 24;
  int id;

  MenuButtons_RemoveSelectable();
  MenuButton_Select(8);
  for (id = 8; id < 12; id++) {
    MenuButton_Add(8, y, &data_021613f8, id);
    y += 16;
  }
  CombatSetup_AddActionButtons();
  MenuButton_EnableTouch(13);
}
