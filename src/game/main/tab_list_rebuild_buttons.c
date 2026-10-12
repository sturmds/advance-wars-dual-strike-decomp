#include "types.h"
#include "game/menu_button.h"
#include "game/tab_list.h"

extern TabListView gMenuState;
extern const MenuButtonTemplate data_0215d4c4;
extern const MenuButtonTemplate data_0215d53c;

/* Replaces the buttons with those of the current page: nine entries at most,
 * four buttons each (id 4 * entry + column). The page ends at an entry whose
 * unk_04 is 0x68. The selection moves to the last entry when it pointed past
 * the end. */
void TabList_RebuildButtons(void) {
  u8 tab;
  int first;
  int selected;
  int count;

  tab = gMenuState.tab;
  first = gMenuState.pageOfTab[tab] * 9;
  selected = Menu_GetSelectedId();

  MenuButtons_RemoveSelectable();
  for (count = 0; count < 9; count++) {
    if (data_0215d6c4[tab][first + count].unk_04 == 0x68) {
      break;
    }
    MenuButton_Add(8, (5 + count * 2) * 8, &data_0215d53c, count * 4);
    MenuButton_Add(176, (5 + count * 2) * 8, &data_0215d4c4, count * 4 + 1);
    MenuButton_Add(192, (5 + count * 2) * 8, &data_0215d4c4, count * 4 + 2);
    MenuButton_Add(208, (5 + count * 2) * 8, &data_0215d4c4, count * 4 + 3);
  }
  if (selected >= count * 4) {
    selected = (selected & 3) + (count - 1) * 4;
    gMenuState.row = selected >> 2;
    MenuButton_Select(selected);
  } else {
    MenuButton_Select(selected);
  }
}
