#include "types.h"
#include "game/keypad.h"

/* TRUE unless B is held. */
BOOL func_020ef2f0(void) {
  if (gKeypad->held & PAD_BUTTON_B) {
    return FALSE;
  }
  return TRUE;
}
