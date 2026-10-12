#include "types.h"
#include "game/keypad.h"

/* Steps *index with the d-pad keys in gKeypad->repeat: right or down
 * forward, left or up back, wrapping within 0 to count - 1. */
void func_020ef260(int* index, int count) {
  if (gKeypad->repeat & (PAD_KEY_RIGHT | PAD_KEY_DOWN)) {
    *index += 1;
    if (*index == count) {
      *index = 0;
    }
  }
  if (gKeypad->repeat & (PAD_KEY_LEFT | PAD_KEY_UP)) {
    if (*index <= 0) {
      *index = count;
    }
    *index -= 1;
  }
}
