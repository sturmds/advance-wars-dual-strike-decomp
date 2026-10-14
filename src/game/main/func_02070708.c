#include "types.h"
#include "game/menu_state.h"

void Sound_PlaySe(int id);
void TabList_RebuildButtons(void);
void TabList_UpdateHelpText(void);

void func_02070708(void) {
  Sound_PlaySe(0x10002);
  gMenuState.tabList.tab = 0;
  TabList_RebuildButtons();
  TabList_UpdateHelpText();
}
