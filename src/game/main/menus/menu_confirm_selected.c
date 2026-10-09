#include "types.h"
#include "game/menu_button.h"

BOOL MenuCursor_HasSelection(void);

void Menu_ConfirmSelected(void)
{
    MenuButton *button;

    if (MenuCursor_HasSelection() == FALSE) {
        return;
    }
    button = data_02158f38->selected;
    if (button->onConfirm != NULL) {
        button->onConfirm(button);
    }
}
