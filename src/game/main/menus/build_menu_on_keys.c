#include "types.h"
#include "game/build_menu.h"
#include "game/menu_button.h"
#include "game/keypad.h"

/* Key callback of the build menu rows. Up and down move within a column and
 * wrap to its other end only on a fresh press, not on auto-repeat. Left and
 * right switch columns when there are two. A confirms, B cancels. */
void BuildMenu_OnKeys(MenuButton* button) {
  int id;
  u16 keys;

  id = button->id;
  keys = gKeypad->repeat;
  if (keys & PAD_KEY_UP) {
    if (id == 0 || id == BUILD_MENU_ROWS) {
      if (keys == gKeypad->pressed) {
        id += BUILD_MENU_ROWS - 1;
        if (id >= gBuildMenuCount - 1) {
          id = gBuildMenuCount - 1;
        }
      }
    } else {
      id--;
    }
  }
  if (keys & PAD_KEY_DOWN) {
    if (id == BUILD_MENU_ROWS - 1 || id == BUILD_MENU_ROWS * 2 - 1 ||
        id == gBuildMenuCount - 1) {
      if (keys == gKeypad->pressed) {
        id -= BUILD_MENU_ROWS - 1;
        if (id < 0) {
          id = 0;
        }
      }
    } else {
      id++;
    }
  }
  if (gBuildMenuCount > BUILD_MENU_ROWS) {
    if (keys & PAD_KEY_LEFT) {
      if (id >= BUILD_MENU_ROWS) {
        id -= BUILD_MENU_ROWS;
      }
    }
    if (keys & PAD_KEY_RIGHT) {
      if (id < BUILD_MENU_ROWS) {
        id += BUILD_MENU_ROWS;
      }
    }
    if (id >= gBuildMenuCount - 1) {
      id = gBuildMenuCount - 1;
    }
  }
  MenuButton_Select(id);
  if (gKeypad->pressed & PAD_BUTTON_A) {
    MenuButton_Confirm(button);
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    Menu_CancelSelected();
  }
}
