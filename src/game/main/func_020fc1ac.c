#include "types.h"
#include "game/keypad.h"

extern int data_022a7e68;
void* func_020fc234(void);

void func_020fc1ac(void) {
  if (data_022a7e68 != 2) {
    return;
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_020fc234();
  }
}
