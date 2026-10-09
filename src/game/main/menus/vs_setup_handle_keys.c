#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

MenuButton *func_02025d58(int id);
MenuButton *func_020262b0(int id);
void func_02025008(void);
BOOL func_02045680(void);
void VsSetup_NextForceRankSet(int a);
void VsSetup_PrevForceRankSet(int a);
void func_02068e6c(MenuButton *button);
void func_020690f4(MenuButton *button);
void func_02069378(int a);
void func_02069424(int a);

/* onKeys of the Vs setup buttons (COs, rules and so on). Left and right
 * step to the previous or next button id that exists, 0-15 with wrap; up
 * and down change the selected button's setting; L and R cycle the Force
 * Rank set of the player slot in BattleSetupState.unk_2e4. */
void VsSetup_HandleKeys(MenuButton *button)
{
    int cursor;

    cursor = button->id;
    if (gKeypad->repeat & PAD_KEY_LEFT) {
        do {
            if (cursor == 0) {
                cursor = 15;
            } else {
                cursor--;
            }
        } while (func_02025d58(cursor) == NULL);
    }
    if (gKeypad->repeat & PAD_KEY_RIGHT) {
        do {
            if (cursor == 15) {
                cursor = 0;
            } else {
                cursor++;
            }
        } while (func_02025d58(cursor) == NULL);
    }
    if (cursor != button->id) {
        button = func_020262b0(cursor);
    }
    if (gKeypad->repeat & PAD_KEY_UP) {
        func_020690f4(button);
    }
    if (gKeypad->repeat & PAD_KEY_DOWN) {
        func_02068e6c(button);
    }
    if (gKeypad->pressed & PAD_BUTTON_L) {
        VsSetup_PrevForceRankSet(0);
    }
    if (gKeypad->pressed & PAD_BUTTON_R) {
        VsSetup_NextForceRankSet(0);
    }
    if ((gKeypad->pressed & PAD_BUTTON_X) && !func_02045680()) {
        func_02069378(0);
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        func_02025008();
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
        func_02069424(0);
    }
}
