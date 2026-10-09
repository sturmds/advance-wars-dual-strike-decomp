#include "types.h"
#include "game/menu_button.h"

int func_02026404(MenuButton *button, int mask);

/* Returns TRUE if the point (x, y) is inside the button. Buttons with
 * flag 0x1000 take every point. */
BOOL MenuButton_ContainsPoint(MenuButton *button, int x, int y)
{
    if (func_02026404(button, 0x1000)) {
        return TRUE;
    }
    if (x < button->x || button->x + button->width - 1 < x) {
        return FALSE;
    }
    if (y < button->y || button->y + button->height - 1 < y) {
        return FALSE;
    }
    return TRUE;
}
