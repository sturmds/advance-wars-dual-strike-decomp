#include "types.h"
#include "game/menu_button.h"
#include "game/tab_list.h"

extern TabListView gMenuState;
void Sound_PlaySe(int id);
void TabList_UpdateHelpText(void);

void TabList_OnSelect(MenuButton* button) {
  Sound_PlaySe(0x10002);
  gMenuState.row = button->id >> 2;
  gMenuState.unk_06 = button->id & 3;
  TabList_UpdateHelpText();
}
