#include "types.h"
#include "game/keypad.h"

/* Folds this frame's held keys into keypad: opposite D-pad directions
 * cancel, then newly pressed keys and auto-repeat are worked out in the
 * next* fields, which are then copied to the public fields at the top. */
void Keypad_UpdateState(Keypad* keypad, u16 keys) {
  if ((keys & (PAD_KEY_RIGHT | PAD_KEY_LEFT)) ==
      (PAD_KEY_RIGHT | PAD_KEY_LEFT)) {
    keys ^= PAD_KEY_RIGHT | PAD_KEY_LEFT;
  }
  if ((keys & (PAD_KEY_UP | PAD_KEY_DOWN)) == (PAD_KEY_UP | PAD_KEY_DOWN)) {
    keys ^= PAD_KEY_UP | PAD_KEY_DOWN;
  }
  keypad->nextPrevHeld = keypad->nextHeld;
  keypad->nextHeld = keys;
  keypad->nextRepeat =
      keypad->nextHeld & (keypad->nextHeld ^ keypad->nextPrevHeld);
  keypad->nextPressed = keypad->nextRepeat;
  if (keypad->nextHeld != 0 && keypad->nextHeld == keypad->nextPrevHeld) {
    keypad->repeatCountdown--;
    if (keypad->repeatCountdown == 0) {
      keypad->nextRepeat = keypad->nextHeld;
      keypad->repeatCountdown = keypad->repeatInterval;
    }
  } else {
    keypad->repeatCountdown = keypad->repeatDelay;
  }
  keypad->held = keypad->nextHeld;
  keypad->pressed = keypad->nextPressed;
  keypad->repeat = keypad->nextRepeat;
  keypad->prevHeld = keypad->nextPrevHeld;
}
