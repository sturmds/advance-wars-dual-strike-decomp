#include "types.h"
#include "game/keypad.h"
#include "game/map_select.h"
#include "game/menu_button.h"

/* Each returns the cursor position after the move. The keys turn the
 * page grid: up and down move along a row of COLS (MapSelect_PosLeft and
 * _PosRight), left and right move between rows. */
int MapSelect_PosLeft(int pos);
int MapSelect_PosRight(int pos);
int MapSelect_PosDown(int pos);
int func_0205ef34(int pos);
void func_0205ec5c(int unused);
void func_0205ec20(int unused);
void MapSelect_NextPage(MenuButton *button);
void MapSelect_PrevPage(MenuButton *button);

/* onKeys of the map buttons on the map select screen. The d-pad moves the
 * cursor through the page (left and right do nothing in the War Room). L
 * and R, when not held together, change the list; Y turns to the previous
 * page and X (MapSelect_NextPage) likely to the next. A confirms the map, B
 * cancels. */
void MapSelect_HandleKeys(MenuButton *button)
{
    int pos;

    pos = button->id;
    if (gKeypad->repeat & PAD_KEY_UP) {
        pos = MapSelect_PosLeft(pos);
    }
    if (gKeypad->repeat & PAD_KEY_DOWN) {
        pos = MapSelect_PosRight(pos);
    }
    if ((gKeypad->repeat & PAD_KEY_LEFT) && STATE->gameMode != GAME_MODE_WAR_ROOM) {
        pos = func_0205ef34(pos);
    }
    if ((gKeypad->repeat & PAD_KEY_RIGHT) && STATE->gameMode != GAME_MODE_WAR_ROOM) {
        pos = MapSelect_PosDown(pos);
    }
    if (pos != button->id) {
        MenuButton_Select(pos);
    }
    if ((gKeypad->held & (PAD_BUTTON_L | PAD_BUTTON_R)) != (PAD_BUTTON_L | PAD_BUTTON_R)) {
        if (gKeypad->repeat & PAD_BUTTON_L) {
            func_0205ec5c(0);
            return;
        }
        if (gKeypad->repeat & PAD_BUTTON_R) {
            func_0205ec20(0);
            return;
        }
    }
    if (gKeypad->pressed & PAD_BUTTON_X) {
        MapSelect_NextPage(NULL);
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_Y) {
        MapSelect_PrevPage(NULL);
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_A) {
        MenuButton_Confirm(button);
        return;
    }
    if (gKeypad->pressed & PAD_BUTTON_B) {
        Menu_CancelSelected();
    }
}
