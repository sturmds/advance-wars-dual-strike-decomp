#include "types.h"
#include "game/menu_button.h"

int MenuButton_TestFlags(MenuButton *entry, int mask);

void func_02025bfc(void)
{
    int i;
    MenuButton *entry;

    entry = gMenuButtons;
    for (i = 0; i < 80; i++, entry++) {
        if (MenuButton_TestFlags(entry, 1) != 1) {
            entry->active = 0;
        }
    }
}
