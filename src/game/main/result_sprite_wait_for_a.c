#include "types.h"
#include "game/keypad.h"

void ResultScreen_Confirm(int button);

/* Script step: confirms the result screen when A is pressed. */
void ResultSprite_WaitForA(void)
{
    if (gKeypad->pressed & PAD_BUTTON_A) {
        ResultScreen_Confirm(0);
    }
}
