#include "types.h"
#include "game/keypad.h"

void func_020754d0(int arg);

void Menu_CancelOnBButton(void) {
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_020754d0(0);
  }
}
