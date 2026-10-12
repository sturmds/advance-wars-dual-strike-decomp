#include "types.h"
#include "game/keypad.h"

/* TRUE when A or Start is set in Keypad.nextPressed. The shop screen skips its
 * points count with it. */
BOOL Keypad_ConfirmNextPressed(void) {
  if (gKeypad->nextPressed & (PAD_BUTTON_A | PAD_BUTTON_START)) {
    return TRUE;
  }
  return FALSE;
}
