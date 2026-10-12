#include "types.h"
#include "game/keypad.h"

u32 func_02024a6c(void) {
  u16 keys = gKeypad->nextPressed;

  if (keys & PAD_BUTTON_A) {
    return 0x10;
  }
  if (keys & PAD_BUTTON_START) {
    return 0x400;
  }
  return 0;
}
