#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

/* Key handler of a row of four buttons (ids 0 to 3): left and right move the cursor with
 * wraparound, A confirms and B cancels. */
void func_02087828(MenuButton* button) {
  int next;
  int keys;
  u8 id;

  id = button->id;
  keys = gKeypad->repeat;
  next = id;
  if (keys & PAD_KEY_LEFT) {
    next = id - 1;
    if (next < 0) {
      next = 3;
    }
  }
  if (keys & PAD_KEY_RIGHT) {
    next++;
    if (next >= 4) {
      next = 0;
    }
  }
  if (next != id) {
    button = MenuButton_Select(next);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
