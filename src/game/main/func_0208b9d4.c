#include "types.h"
#include "game/keypad.h"

void func_0208ba10(int arg);

void func_0208b9d4(void) {
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_0208ba10(0);
  }
}
