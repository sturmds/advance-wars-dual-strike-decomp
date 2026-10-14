#include "types.h"
#include "game/menu_state.h"
#include "game/tab_list.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
void TabList_RebuildButtons(void);
int TabList_GetPageCount(int tab);
void TabList_UpdateHelpText(void);

/* Shows the previous page of the current tab, wrapping to the last page. */
void TabList_PrevPage(MenuButton* button) {
  int pageCount = TabList_GetPageCount(gMenuState.tabList.tab);

  if (gMenuState.tabList.pageOfTab[gMenuState.tabList.tab] == 0) {
    gMenuState.tabList.pageOfTab[gMenuState.tabList.tab] = pageCount - 1;
  } else {
    gMenuState.tabList.pageOfTab[gMenuState.tabList.tab]--;
  }
  Sound_PlaySe(0x10002);
  TabList_RebuildButtons();
  TabList_UpdateHelpText();
}
