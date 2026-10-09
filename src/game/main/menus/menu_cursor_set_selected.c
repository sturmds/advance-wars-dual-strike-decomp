#include "types.h"
#include "game/menu_button.h"

void MenuButton_NotifySelected(MenuButton *button);

/* Puts the cursor on a button. When the cursor moves off a selectable
 * button to a different one, MenuButton_NotifySelected runs the new button's
 * callback at 0x34. */
MenuButton *MenuCursor_SetSelected(MenuButton *button, int id)
{
    BOOL changed;

    if (MenuCursor_HasSelection() && button != NULL && data_02158f38->selected != button) {
        changed = TRUE;
    } else {
        changed = FALSE;
    }
    data_02158f38->selected = button;
    data_02158f38->selectedId = id;
    if (changed) {
        MenuButton_NotifySelected(data_02158f38->selected);
    }
    return data_02158f38->selected;
}
