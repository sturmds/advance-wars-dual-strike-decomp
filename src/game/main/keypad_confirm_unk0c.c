#include "types.h"
#include "game/keypad.h"

/* TRUE when A or Start is set in Keypad.unk_0c. The shop screen skips its
 * points count with it. */
BOOL Keypad_ConfirmUnk0c(void)
{
    if (gKeypad->unk_0c & (PAD_BUTTON_A | PAD_BUTTON_START)) {
        return TRUE;
    }
    return FALSE;
}
