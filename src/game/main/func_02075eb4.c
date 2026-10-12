#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void func_02075f54(int arg);
void func_02075f60(int arg);

/* Six-entry menu keys: up and down move the selection, wrapping at the ends;
 * A and B run the two handlers. */
void func_02075eb4(u8* selected) {
  u32 repeat;
  u8 current;
  int next;
  current = *selected;
  repeat = gKeypad->repeat;
  next = current;

  if (repeat & PAD_KEY_UP) {
    if (current != 0) {
      next = current - 1;
    } else {
      next = 5;
    }
  }
  if (repeat & PAD_KEY_DOWN) {
    if (next == 5) {
      next = 0;
    } else {
      next++;
    }
  }
  if (next != current) {
    MenuButton_Select(next);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    func_02075f54(0);
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_02075f60(0);
  }
}
