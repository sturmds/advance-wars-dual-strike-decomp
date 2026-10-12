#include "types.h"
#include "game/keypad.h"

extern Keypad data_0216e4e4
    [9]; /* the last entry is data_0216e594, the one gKeypad points to */
extern Keypad data_0216e594;

void Keypad_Init(void) {
  int i;
  Keypad* key = data_0216e4e4;

  for (i = 0; i < 9; i++, key++) {
    key->prevHeld = 0;
    key->held = 0;
    key->pressed = 0;
    key->repeatCountdown = 0;
    key->repeat = 0;
    key->nextPrevHeld = 0;
    key->nextHeld = 0;
    key->nextPressed = 0;
    key->nextRepeat = 0;
    key->repeatDelay = 12;
    key->repeatInterval = 4;
  }
  gKeypad = &data_0216e594;
}
