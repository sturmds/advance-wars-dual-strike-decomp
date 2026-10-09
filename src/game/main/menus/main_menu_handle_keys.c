#include "types.h"
#include "game/keypad.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

MenuButton *func_020262b0(int id);
void func_020250a8(MenuButton *button);
void func_02025008(void);
int func_0203142c(int page);
void func_0202e7b8(int page, int slide);

/* onKeys of the main menu buttons: up and down move the cursor past hidden
 * entries, left, right, L and R flip the page (func_0203142c picks the side),
 * A and B confirm and cancel. */
void MainMenu_HandleKeys(MenuButton *button)
{
    MainMenuPage *page;
    u16 keys;
    int cursor;

    cursor = button->id;
    keys = data_0216e4e0->repeat;
    page = *data_0229c71c;
    if (keys & PAD_KEY_UP) {
        do {
            if (cursor == 0) {
                cursor = page->entryCount - 1;
            } else {
                cursor--;
            }
        } while (page->entries[cursor].hidden);
    }
    if (keys & PAD_KEY_DOWN) {
        do {
            if (cursor == page->entryCount - 1) {
                cursor = 0;
            } else {
                cursor++;
            }
        } while (page->entries[cursor].hidden);
    }
    if (keys & (PAD_KEY_LEFT | PAD_BUTTON_L)) {
        data_021599d0->cursors[data_021599d0->page] = cursor;
        if (func_0203142c(data_021599d0->page) == 1) {
            func_0202e7b8(6, 2);
        } else {
            func_0202e7b8(0, 2);
        }
        return;
    }
    if (keys & (PAD_KEY_RIGHT | PAD_BUTTON_R)) {
        data_021599d0->cursors[data_021599d0->page] = cursor;
        if (func_0203142c(data_021599d0->page) == 1) {
            func_0202e7b8(6, 3);
        } else {
            func_0202e7b8(0, 3);
        }
        return;
    }
    if (cursor != button->id) {
        button = func_020262b0(cursor);
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_A) {
        func_020250a8(button);
        return;
    }
    if (data_0216e4e0->pressed & PAD_BUTTON_B) {
        func_02025008();
    }
}
