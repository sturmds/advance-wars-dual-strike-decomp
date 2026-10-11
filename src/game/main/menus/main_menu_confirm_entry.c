#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"
#include "game/unlocks.h"

void Sound_PlaySe(int id);
void MainMenu_StartPageFlip(int page, int slide);
void func_0202d6c0(int mode);
void func_020b9890(void);
void func_0202e868(void);

extern int data_022a7fa8;             /* next game mode, index into the table func_02102690 runs */

/* onConfirm of the main menu buttons. The entry's action (unk_00) either
 * flips to another menu page or picks the next game mode; actions 11, 12,
 * 25 and 26 depend on the current page. Picking a mode closes the menu. */
void MainMenu_ConfirmEntry(MenuButton *button)
{
    MainMenuPage *page = *data_0229c71c;

    Sound_PlaySe(0x10006);
    data_021599d0->cursors[data_021599d0->page] = button->id;
    switch (page->entries[button->id].unk_00) {
    case 0:
        MainMenu_StartPageFlip(1, 1);
        return;
    case 1:
        MainMenu_StartPageFlip(2, 1);
        return;
    case 2:
        MainMenu_StartPageFlip(3, 1);
        return;
    case 3:
        MainMenu_StartPageFlip(4, 1);
        return;
    case 4:
        MainMenu_StartPageFlip(5, 1);
        return;
    case 5:
        data_022a7fa8 = 21;
        break;
    case 6:
        data_022a7fa8 = 31;
        break;
    case 7:
        MainMenu_StartPageFlip(7, 1);
        return;
    case 8:
        data_022a7fa8 = 20;
        break;
    case 9:
        data_022a7fa8 = 19;
        break;
    case 10:
        MainMenu_StartPageFlip(8, 1);
        return;
    case 11:
        switch (data_021599d0->page) {
        case 1:
            if (GameFlag_IsSet(32)) {
                MainMenu_StartPageFlip(11, 1);
                return;
            }
            data_022a7fa8 = 0;
            break;
        case 2:
            data_022a7fa8 = 12;
            break;
        case 3:
            MainMenu_StartPageFlip(13, 1);
            return;
        case 4:
            MainMenu_StartPageFlip(9, 1);
            return;
        case 5:
            if (GameFlag_IsSet(41)) {
                MainMenu_StartPageFlip(12, 1);
                return;
            }
            data_022a7fa8 = 14;
            break;
        }
        break;
    case 12:
        switch (data_021599d0->page) {
        case 1:
            MainMenu_StartPageFlip(10, 1);
            return;
        case 2:
            data_022a7fa8 = 13;
            break;
        case 3:
            data_022a7fa8 = 7;
            break;
        case 4:
            data_022a7fa8 = 11;
            break;
        case 5:
            data_022a7fa8 = 17;
            break;
        }
        break;
    case 13:
        data_022a7fa8 = 8;
        break;
    case 14:
        data_022a7fa8 = 9;
        break;
    case 15:
        data_022a7fa8 = 10;
        break;
    case 16:
        data_022a7fa8 = 29;
        break;
    case 17:
        data_022a7fa8 = 28;
        break;
    case 18:
        data_022a7fa8 = 30;
        break;
    case 19:
        data_022a7fa8 = 22;
        break;
    case 20:
        data_022a7fa8 = 23;
        break;
    case 21:
        data_022a7fa8 = 24;
        break;
    case 22:
        data_022a7fa8 = 25;
        break;
    case 23:
        data_022a7fa8 = 26;
        break;
    case 24:
        data_022a7fa8 = 27;
        break;
    case 25:
        switch (data_021599d0->page) {
        case 11:
            data_022a7fa8 = 0;
            break;
        case 12:
            data_022a7fa8 = 14;
            break;
        }
        break;
    case 26:
        switch (data_021599d0->page) {
        case 11:
            data_022a7fa8 = 1;
            break;
        case 12:
            data_022a7fa8 = 15;
            break;
        }
        break;
    case 27:
        data_022a7fa8 = 16;
        break;
    case 28:
        data_022a7fa8 = 18;
        break;
    case 29:
        data_022a7fa8 = 5;
        break;
    case 30:
        data_022a7fa8 = 6;
        break;
    case 31:
        data_022a7fa8 = 2;
        break;
    case 32:
        data_022a7fa8 = 3;
        break;
    case 33:
        data_022a7fa8 = 4;
        break;
    default:
        return;
    }
    func_0202d6c0(data_022a7fa8);
    MenuButtons_RemoveSelectable();
    MenuButtons_CloseAll();
    func_020b9890();
    func_0202e868();
}
