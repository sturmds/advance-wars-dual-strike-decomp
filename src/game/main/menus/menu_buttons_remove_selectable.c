#include "types.h"
#include "game/menu_button.h"

int func_02026404(MenuButton *button, int mask);

/* Removes every selectable button at once, without the closing animation. */
void MenuButtons_RemoveSelectable(void)
{
    int i;
    MenuButton *button = gMenuButtons;

    for (i = 0; i < 80; i++, button++) {
        if (func_02026404(button, MENU_BUTTON_SELECT_ON_TOUCH)) {
            button->active = 0;
        }
    }
}
