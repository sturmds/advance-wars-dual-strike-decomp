#include "types.h"
#include "game/menu_button.h"

int func_02026404(MenuButton *button, int mask);

/* Stops touch reaching every button that is not selectable. */
void MenuButtons_DisableTouch(void)
{
    int i;
    MenuButton *button = gMenuButtons;

    for (i = 0; i < 80; i++, button++) {
        if (func_02026404(button, MENU_BUTTON_SELECT_ON_TOUCH) != MENU_BUTTON_SELECT_ON_TOUCH) {
            button->unk_1c |= MENU_BUTTON_NO_TOUCH_CONFIRM;
        }
    }
}
