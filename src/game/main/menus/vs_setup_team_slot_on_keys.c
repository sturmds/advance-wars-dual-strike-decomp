#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"

void func_02066fc0(MenuButton *button);
void func_02067088(MenuButton *button);
void func_0206714c(int unused);

#define FIRST_SLOT_BUTTON 48
#define LAST_SLOT_BUTTON  51

/* Key handler of the player slot buttons 48-51 of the Vs setup screen.
 * Left and right move to the previous or next slot button that exists,
 * wrapping. Up and down change the slot's team (func_02067088 and
 * func_02066fc0 step setup.unk_2c8[slot]). B cancels; A goes on through
 * func_0206714c. */
void VsSetup_TeamSlotOnKeys(MenuButton *button)
{
    int id = button->id;

    if (gKeypad->repeat & PAD_KEY_LEFT) {
        do {
            if (id == FIRST_SLOT_BUTTON) {
                id = LAST_SLOT_BUTTON;
            } else {
                id--;
            }
        } while (MenuButton_Find(id) == NULL);
    }
    if (gKeypad->repeat & PAD_KEY_RIGHT) {
        do {
            if (id == LAST_SLOT_BUTTON) {
                id = FIRST_SLOT_BUTTON;
            } else {
                id++;
            }
        } while (MenuButton_Find(id) == NULL);
    }
    if (id != button->id) {
        button = MenuButton_Select(id);
    }
    if (gKeypad->repeat & PAD_KEY_UP) {
        func_02067088(button);
    }
    if (gKeypad->repeat & PAD_KEY_DOWN) {
        func_02066fc0(button);
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        Menu_CancelSelected();
    } else if (gKeypad->pressed & PAD_BUTTON_A) {
        func_0206714c(0);
    }
}
