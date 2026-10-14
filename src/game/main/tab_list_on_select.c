#include "types.h"
#include "game/menu_state.h"
#include "game/menu_button.h"
#include "game/tab_list.h"

void Sound_PlaySe(int id);
void TabList_UpdateHelpText(void);

void TabList_OnSelect(MenuButton* button) {
  Sound_PlaySe(0x10002);
  gMenuState.tabList.row = button->id >> 2;
  gMenuState.tabList.unk_06 = button->id & 3;
  TabList_UpdateHelpText();
}
