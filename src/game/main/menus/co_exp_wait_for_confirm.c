#include "types.h"
#include "game/co_exp.h"
#include "game/keypad.h"
#include "game/menu_state.h"

void ResultScreen_Confirm(int unused);
void func_02033c90(CoExp *exp, int a, int b, int c);

extern MenuState gMenuState;

/* Task step: redraws the CO cells, and on A, B or Start calls
 * ResultScreen_Confirm (the original passes it a 0 it ignores). */
void CoExp_WaitForConfirm(void)
{
    func_02033c90((CoExp *)((u8 *)&gMenuState + (u32)&((MenuState *)0)->coExp.exp), 0, 0, -1);
    if (gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) {
        ResultScreen_Confirm(0);
    }
}
