#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"


extern u8 data_0216e4d0;

/* Calls onKeys of the selected button when a key is down, unless
 * data_0216e4d0 is set. */
void Menu_DispatchKeys(void)
{
    MenuButton *button;

    if (!MenuCursor_HasSelection()) {
        return;
    }
    button = data_02158f38->selected;
    if (button->slideState != 2) {
        return;
    }
    if (gKeypad->repeat == 0) {
        return;
    }
    if (data_0216e4d0 != 0) {
        return;
    }
    if (button->onKeys != NULL) {
        button->onKeys(button);
    }
}
