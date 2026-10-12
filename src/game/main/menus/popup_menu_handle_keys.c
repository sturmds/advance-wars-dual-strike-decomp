#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"

/* onKeys of a pop-up menu button: up and down move the cursor and wrap round
 * at either end, but only on a fresh press, not on auto-repeat. A confirms
 * and B cancels when the cursor did not move. */
void PopupMenu_HandleKeys(PopupMenuTask* menu, MenuButton* button) {
  int id = button->id;

  if (gKeypad->repeat & PAD_KEY_UP) {
    if (button->id == 0) {
      if (gKeypad->repeat != gKeypad->pressed) {
        return;
      }
      id = menu->count;
    }
    id--;
  } else if (gKeypad->repeat & PAD_KEY_DOWN) {
    if (button->id == menu->count - 1) {
      if (gKeypad->repeat != gKeypad->pressed) {
        return;
      }
      id = -1;
    }
    id++;
  }
  if (id != button->id) {
    MenuButton_Select(id);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
