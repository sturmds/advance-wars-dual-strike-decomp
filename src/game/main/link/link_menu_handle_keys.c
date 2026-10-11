#include "types.h"
#include "game/keypad.h"
#include "game/Unk_0229fb30.h"

void func_02086b1c(u8 entry);
void func_02086b30(void);
void func_02086b44(void);
void func_02086b58(void);
void func_02086b6c(void);

/* Keys of player 0 on the link menu, which has five entries
 * (data_0229fb30.unk_45b is the selected one). Left and right pick the
 * previous and next entry, wrapping around, and send message 6. Up sends 7
 * and down 8 (LinkMenu_Update raises and lowers the setting). A sends 4 and
 * B sends 5. */
void LinkMenu_HandleKeys(void)
{
    s8 entry;

    entry = data_0229fb30.unk_45b;
    if (gKeypad->repeat & PAD_KEY_LEFT) {
        entry--;
        if (entry < 0) {
            entry = 4;
        }
        func_02086b1c(entry);
    }
    if (gKeypad->repeat & PAD_KEY_RIGHT) {
        entry++;
        if (entry >= 5) {
            entry = 0;
        }
        func_02086b1c(entry);
    }
    if (gKeypad->repeat & PAD_KEY_UP) {
        func_02086b6c();
    }
    if (gKeypad->repeat & PAD_KEY_DOWN) {
        func_02086b58();
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
        func_02086b44();
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        func_02086b30();
    }
}
