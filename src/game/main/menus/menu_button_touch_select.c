#include "types.h"
#include "game/menu_button.h"
#include "game/touch.h"

BOOL func_0201ad28(void);
BOOL func_0202576c(MenuButton *button, int x, int y);
void func_020262a0(MenuButton *button);
int func_02026404(MenuButton *button, int mask);

/* Selects the button when the touch point is inside it. Returns TRUE if
 * this frame's touch selected it. */
BOOL MenuButton_TouchSelect(MenuButton *button)
{
    int x = data_0216e4d0.x;
    int y = data_0216e4d0.y;

    if (func_0201ad28()) {
        return FALSE;
    }
    if (data_0216e4d0.phase == 0) {
        return FALSE;
    }
    if (func_02026404(button, 0x1) == 0) {
        return FALSE;
    }
    if (!func_0202576c(button, x, y)) {
        return FALSE;
    }
    if (data_02158f38->selected == button) {
        return FALSE;
    }
    if (func_02026404(button, 0x400)) {
        return FALSE;
    }
    func_020262a0(button);
    return TRUE;
}
