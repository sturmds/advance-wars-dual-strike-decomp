#include "types.h"
#include "game/menu_button.h"
#include "game/touch.h"

int func_02026404(MenuButton *entry, int mask);
u8 func_020263f4(MenuButton *button);
void func_020263fc(MenuButton *button, int touchState);
BOOL MenuButton_ContainsPoint(MenuButton *button, int x, int y);
void func_020262a0(MenuButton *button);
BOOL func_0201ad28(void);

/* Needed to match: the original builds this test as a 0/1 value first,
 * as an inline function does. */
static inline BOOL Touch_Is(int phase)
{
    if (data_0216e4d0.active != 0 && data_0216e4d0.phase == phase) {
        return TRUE;
    }
    return FALSE;
}

static inline int Abs(int value)
{
    return value < 0 ? -value : value;
}

/* Touch handling of a shown button, once per frame. Returns TRUE when the
 * touch confirmed the button: pressed and released on it, with the release
 * at most 16 pixels from where the touch began. */
BOOL MenuButton_HandleTouch(MenuButton *button)
{
    u8 x = data_0216e4d0.x;
    u8 y = data_0216e4d0.y;
    u8 startX = data_0216e4d0.startX;
    u8 startY = data_0216e4d0.startY;

    switch (func_020263f4(button)) {
    case MENU_BUTTON_TOUCH_HELD:   /* needed to match: this case comes first */
        if (MenuButton_ContainsPoint(button, x, y) == FALSE) {
            func_020263fc(button, MENU_BUTTON_TOUCH_NONE);
            break;
        }
        if (Touch_Is(TOUCH_RELEASED)) {
            func_020263fc(button, MENU_BUTTON_TOUCH_NONE);
        }
        break;
    case MENU_BUTTON_TOUCH_NONE:
        if (Touch_Is(TOUCH_PRESSED) && MenuButton_ContainsPoint(button, x, y) == TRUE) {
            func_020263fc(button, MENU_BUTTON_TOUCH_PRESSED);
        }
        break;
    case MENU_BUTTON_TOUCH_PRESSED:
        if (MenuButton_ContainsPoint(button, x, y) == FALSE) {
            func_020263fc(button, MENU_BUTTON_TOUCH_NONE);
            break;
        }
        if (Touch_Is(TOUCH_RELEASED) == TRUE) {
            func_020263fc(button, MENU_BUTTON_TOUCH_NONE);
            if (func_02026404(button, MENU_BUTTON_NO_TOUCH_CONFIRM) != 0) {
                return FALSE;
            }
            if (func_0201ad28() != FALSE) {
                return FALSE;
            }
            if (func_02026404(button, MENU_BUTTON_SELECT_ON_TOUCH) != 0) {
                func_020262a0(button);
            }
            if (Abs(x - startX) > 16 || Abs(y - startY) > 16) {
                return FALSE;
            }
            if (button->onConfirm != NULL) {
                button->onConfirm(button);
            }
            return TRUE;
        }
        if (Touch_Is(TOUCH_HELD)) {
            func_020263fc(button, MENU_BUTTON_TOUCH_HELD);
        }
        break;
    }
    return FALSE;
}
