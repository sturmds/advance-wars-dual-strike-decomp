#include "types.h"
#include "game/menu_button.h"
#include "game/keypad.h"

/* onKeys of the row buttons: up and down move the selection through ids 0-4, wrapping. */
void LinkMenu_OnKeys(MenuButton* button) {
  u32 keys;
  u8 id;
  int next;

  id = button->id;
  keys = gKeypad->repeat;
  next = id;

  if (keys & PAD_KEY_UP) {
    if (id == 0) {
      next = 4;
    } else {
      next = id - 1;
    }
  }
  if (keys & PAD_KEY_DOWN) {
    if (next == 4) {
      next = 0;
    } else {
      next++;
    }
  }
  if (next == id) {
    return;
  }
  MenuButton_Select(next);
}
