#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/touch.h"

/* Calls onKeys of the selected button when a key is down, unless
 * the screen is touched. */
void Menu_DispatchKeys(void) {
  MenuButton* button;

  if (!MenuCursor_HasSelection()) {
    return;
  }
  button = gMenuCursor->selected;
  if (button->slideState != 2) {
    return;
  }
  if (gKeypad->repeat == 0) {
    return;
  }
  if (gTouch.phase != TOUCH_RELEASED) {
    return;
  }
  if (button->onKeys != NULL) {
    button->onKeys(button);
  }
}
