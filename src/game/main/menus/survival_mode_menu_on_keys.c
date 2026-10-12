#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void func_02046f3c(void);
int SurvivalMode_HasSecondRow(void);

/* Key handler of the two rows of the Survival mode menu. Up or down moves
 * to the other row when SurvivalMode_HasSecondRow reports the second row is on offer.
 * R calls func_02046f3c, A confirms and B cancels the selected button. */
void SurvivalModeMenu_OnKeys(MenuButton* button) {
  int row = button->id;

  if (SurvivalMode_HasSecondRow() == 1) {
    if (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN)) {
      row = (row + 1) & 1;
    }
  }
  if (row != button->id) {
    button = MenuButton_Select(row);
  }
  if (gKeypad->pressed & PAD_BUTTON_R) {
    func_02046f3c();
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
