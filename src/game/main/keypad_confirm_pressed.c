#include "types.h"
#include "game/keypad.h"

/* TRUE when A or Start was pressed this frame. */
BOOL Keypad_ConfirmPressed(void) {
  if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_START)) {
    return TRUE;
  }
  return FALSE;
}
