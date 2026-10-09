#include "types.h"
#include "game/menu_button.h"

/* Deactivates every menu button and resets the cursor: sprite tile 0x3b8,
 * palette 15, nothing selected, screen 0. */
void Menu_Reset(void)
{
    int i;
    MenuButton *button;

    button = gMenuButtons;
    for (i = 0; i < 80; i++, button++) {
        button->active = 0;
    }
    data_02158f38->tile = 0x3b8;
    data_02158f38->palette = 15;
    data_02158f38->selected = NULL;
    data_02158f38->selectedId = 0;
    data_02158f38->screen = 0;
}
