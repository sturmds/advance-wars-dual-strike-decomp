#include "types.h"
#include "game/menu_state.h"
#include "game/tab_list.h"

void HelpText_Clear(void);
void HelpText_Show(u32 textId, int tile);
BOOL TabList_SelectionHasHelp(void);

/* Shows the help text of the selected button, or clears it when that button
 * has none (see TabList_SelectionHasHelp). */
void TabList_UpdateHelpText(void) {
  u8 tab;
  u8 row;
  int entry;
  u8 col;
  tab = gMenuState.tabList.tab;
  row = gMenuState.tabList.row;
  col = gMenuState.tabList.unk_06;
  entry = gMenuState.tabList.pageOfTab[tab] * 9 + row;

  if (!TabList_SelectionHasHelp()) {
    HelpText_Clear();
    return;
  }
  HelpText_Show(data_0215d6c4[tab][entry].helpTextId[col], 192);
}
