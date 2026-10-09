#include "types.h"
#include "game/menu_button.h"

/* Sets flag 0x400 of the active button with this id; MenuButton_EnableTouch clears it. */
void func_02025d1c(int id)
{
    MenuButton *button;

    button = MenuButton_Find(id);
    if (button != NULL) {
        button->unk_1c |= 0x400;
    }
}
