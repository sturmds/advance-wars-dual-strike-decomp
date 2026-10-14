#include "types.h"
#include "game/menu_state.h"
#include "game/tab_list.h"

int TabList_CallUnk1c(int row, int col);

/* Whether the selected button has help text: not for an entry whose unk_1c
 * function returns 1; always for column 0; for medal column n only once the
 * entry value reaches its threshold. */
BOOL TabList_SelectionHasHelp(void) {
  int tab;
  int entry;
  u8 col;
  u32 value;
  tab = gMenuState.tabList.tab;
  col = gMenuState.tabList.unk_06;
  entry = gMenuState.tabList.pageOfTab[tab] * 9 + gMenuState.tabList.row;

  if (TabList_CallUnk1c(tab, entry) == 1) {
    return FALSE;
  }
  switch (col) {
    case 0:
      return TRUE;
    case 1:
    case 2:
    case 3:
      value = data_0215d6c4[tab][entry].getValue(tab, entry);
      return value >= data_0215d6c4[tab][entry].medalThreshold[col - 1];
    default:
      return FALSE;
  }
}
