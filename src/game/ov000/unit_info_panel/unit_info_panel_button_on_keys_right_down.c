#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void UnitInfoPanel_ButtonOnKeysRightDown(MenuButton* button) {
  if (gKeypad->repeat & (PAD_KEY_LEFT | PAD_KEY_UP)) {
    return;
  }
  if (gKeypad->repeat & (PAD_KEY_RIGHT | PAD_KEY_DOWN)) {
    MenuButton_Confirm(button);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
