#include "types.h"
#include "game/tab_list.h"
#include "game/menu_button.h"

extern TabListView gMenuState;
void Sound_PlaySe(int id);
void TabList_RebuildButtons(void);
int TabList_GetPageCount(int tab);
void TabList_UpdateHelpText(void);

/* Shows the previous page of the current tab, wrapping to the last page. */
void TabList_PrevPage(MenuButton* button) {
  int pageCount = TabList_GetPageCount(gMenuState.tab);

  if (gMenuState.pageOfTab[gMenuState.tab] == 0) {
    gMenuState.pageOfTab[gMenuState.tab] = pageCount - 1;
  } else {
    gMenuState.pageOfTab[gMenuState.tab]--;
  }
  Sound_PlaySe(0x10002);
  TabList_RebuildButtons();
  TabList_UpdateHelpText();
}
