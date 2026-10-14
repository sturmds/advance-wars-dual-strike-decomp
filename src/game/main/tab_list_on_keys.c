#include "types.h"
#include "game/menu_state.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/tab_list.h"

void Sound_PlaySe(int id);
void TabList_RebuildButtons(void);
void TabList_UpdateHelpText(void);
void TabList_PrevPage(MenuButton* button);
void TabList_NextPage(MenuButton* button);
void func_02070800(MenuButton* button);

/* Needed to match: a volatile read of the tab, so that the original's second
 * read in the else branch is not merged with it. */
#define TAB_RELOAD (*(volatile u8*)&gMenuState.tabList.tab)

/* onKeys of the tab list's buttons (id 4 * row + column). The d-pad moves
 * the selection: up and down by rows, down only onto an entry that exists
 * (at most nine rows), left and right wrapping within the four columns.
 * L and R change tab, wrapping over the four tabs; Y and X turn the page;
 * B goes to func_02070800. */
void TabList_OnKeys(MenuButton* button) {
  int repeat;
  int next;
  u8 id;
  int row;

  id = button->id;
  repeat = gKeypad->repeat;
  next = id;
  if (repeat & PAD_KEY_UP) {
    if ((id >> 2) > 0) {
      next = id - 4;
    }
  }
  if (repeat & PAD_KEY_DOWN) {
    row = next >> 2;
    if (row < 8) {
      if (data_0215d6c4[gMenuState.tabList.tab]
                       [gMenuState.tabList.pageOfTab[gMenuState.tabList.tab] *
                            9 +
                        row + 1]
                           .unk_04 != TAB_LIST_END_MARK) {
        next += 4;
      }
    }
  }
  if (repeat & PAD_KEY_LEFT) {
    if ((next & 3) == 0) {
      next += 3;
    } else {
      next--;
    }
  }
  if (repeat & PAD_KEY_RIGHT) {
    if ((next & 3) == 3) {
      next -= 3;
    } else {
      next++;
    }
  }
  if (next != id) {
    MenuButton_Select(next);
  }
  if (gKeypad->pressed & PAD_BUTTON_L) {
    Sound_PlaySe(0x10002);
    if (TAB_RELOAD == 0) {
      gMenuState.tabList.tab = 3;
    } else {
      gMenuState.tabList.tab--;
    }
    TabList_RebuildButtons();
    TabList_UpdateHelpText();
  }
  if (gKeypad->pressed & PAD_BUTTON_R) {
    Sound_PlaySe(0x10002);
    if (TAB_RELOAD == 3) {
      gMenuState.tabList.tab = 0;
    } else {
      gMenuState.tabList.tab++;
    }
    TabList_RebuildButtons();
    TabList_UpdateHelpText();
  }
  if (gKeypad->pressed & PAD_BUTTON_Y) {
    TabList_PrevPage(NULL);
  }
  if (gKeypad->pressed & PAD_BUTTON_X) {
    TabList_NextPage(NULL);
  }
  if (gKeypad->pressed & PAD_BUTTON_B) {
    func_02070800(NULL);
  }
}
