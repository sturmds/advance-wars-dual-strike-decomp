#include "types.h"
#include "game/menu_button.h"

BOOL func_0202634c(void);

void Menu_ConfirmSelected(void)
{
    MenuButton *button;

    if (func_0202634c() == FALSE) {
        return;
    }
    button = data_02158f38->selected;
    if (button->onConfirm != NULL) {
        button->onConfirm(button);
    }
}
