#include "types.h"
#include "game/co_select_screen.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
void func_020418e0(void);
void func_02041944(void);
void CoSelectScreen_BeginExit(void);
void func_020b9890(void);

/* onKeys of the CO select buttons. The cursor wraps around the grid; A and
 * B confirm and cancel, X calls func_02041944, R func_020418e0, and Start
 * plays sound 0x10006 and leaves through CoSelectScreen_BeginExit. */
void CoSelectScreen_HandleKeys(MenuButton *button)
{
    int cursor;
    u16 keys;

    cursor = button->id;
    keys = gKeypad->repeat;
    if ((keys & (PAD_KEY_UP | PAD_KEY_DOWN)) && data_0229c73c->rowCount == 2) {
        if (cursor & 1) {
            cursor = cursor - 1;
        } else {
            cursor = cursor + 1;
        }
    }
    if (keys & PAD_KEY_LEFT) {
        if (cursor >> 1) {
            cursor -= 2;
        } else {
            cursor = ((data_0229c73c->columnCount - 1) << 1) | (cursor & 1);
        }
    }
    if (keys & PAD_KEY_RIGHT) {
        if ((cursor >> 1) == data_0229c73c->columnCount - 1) {
            cursor &= 1;
        } else {
            cursor += 2;
        }
    }
    if (cursor != button->id) {
        button = MenuButton_Select(cursor);
    }
    keys = gKeypad->pressed;
    if (keys & PAD_BUTTON_A) {
        MenuButton_Confirm(button);
        return;
    }
    if (keys & PAD_BUTTON_B) {
        Menu_CancelSelected();
        return;
    }
    if (keys & PAD_BUTTON_X) {
        func_02041944();
        return;
    }
    if (keys & PAD_BUTTON_R) {
        func_020418e0();
        return;
    }
    if (keys & PAD_BUTTON_START) {
        Sound_PlaySe(0x10006);
        CoSelectScreen_BeginExit();
        func_020b9890();
    }
}
