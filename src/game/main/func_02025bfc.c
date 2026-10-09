#include "types.h"
#include "game/menu_button.h"

int func_02026404(MenuButton *entry, int mask);

void func_02025bfc(void)
{
    int i;
    MenuButton *entry;

    entry = gMenuButtons;
    for (i = 0; i < 80; i++, entry++) {
        if (func_02026404(entry, 1) != 1) {
            entry->active = 0;
        }
    }
}
