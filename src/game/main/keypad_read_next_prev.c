#include "types.h"
#include "game/keypad.h"

/* 8 for right or down, 4 for left or up (with key repeat), else 0. */
u32 Keypad_ReadNextPrev(void) {
  u16 keys = gKeypad->repeat;

  if (keys & (PAD_KEY_RIGHT | PAD_KEY_DOWN)) {
    return 8;
  }
  if (keys & (PAD_KEY_LEFT | PAD_KEY_UP)) {
    return 4;
  }
  return 0;
}
