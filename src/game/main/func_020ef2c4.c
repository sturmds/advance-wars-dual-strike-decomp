#include "types.h"
#include "game/keypad.h"

u32 func_020ef2c4(void) {
  u16 keys = gKeypad->pressed;

  if (keys & PAD_BUTTON_A) {
    return 0x10;
  }
  if (keys & PAD_BUTTON_B) {
    return 0x20;
  }
  return 0;
}
