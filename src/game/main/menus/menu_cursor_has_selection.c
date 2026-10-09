#include "types.h"
#include "game/menu_button.h"

/* TRUE when the cursor is on an active button whose unk_04 is not 3 or 4. */
BOOL MenuCursor_HasSelection(void)
{
    MenuButton *button;

    button = data_02158f38->selected;
    if (button == NULL || !button->active || button->slideState == 3 || button->slideState == 4) {
        return FALSE;
    }
    return TRUE;
}
