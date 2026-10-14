#include "types.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/tab_list.h"

extern Unk_027c0280* data_027c0280;

void Gfx_ClearLayerMap(int layer);
void Bg_PutSkillSlot(u16 x, u16 y, u16* map, int palette);
void Bg_PutTile5Quad(u16 x, u16 y, u16* map, int palette);
int TabList_CallUnk1c(int row, int col);
void TabList_DrawMedals(int x, int y, int row, int col);
void Gfx_RequestLayerCopy(int layer);

/* Draws the current page of the tab list: for each of up to nine entries,
 * the three medal slots on layer 1 (columns 22, 24 and 26), then the entry
 * itself and its medals unless its unk_1c function returns 1. Rows past
 * the last entry get blank tiles. */
void TabList_DrawPage(void) {
  int tab;
  int row;
  int first;
  int y;
  TabListEntry* item;
  int entry;

  tab = gMenuState.tabList.tab;
  first = gMenuState.tabList.pageOfTab[tab] * 9;
  Gfx_ClearLayerMap(0);
  for (row = 0, y = 5; row < TAB_LIST_PAGE_ROWS; row++, y += 2) {
    entry = first + row;
    item = &data_0215d6c4[tab][entry];
    if (item->unk_04 == TAB_LIST_END_MARK) {
      break;
    }
    Bg_PutSkillSlot(22, y, data_027c0280->layer1Map, 0);
    Bg_PutSkillSlot(24, y, data_027c0280->layer1Map, 0);
    Bg_PutSkillSlot(26, y, data_027c0280->layer1Map, 0);
    if (TabList_CallUnk1c(tab, entry) != 1) {
      item->draw(1, y, tab, entry);
      TabList_DrawMedals(1, y, tab, entry);
    }
  }
  for (; row < TAB_LIST_PAGE_ROWS; row++) {
    Bg_PutTile5Quad(22, row * 2 + 5, data_027c0280->layer1Map, 0);
    Bg_PutTile5Quad(24, row * 2 + 5, data_027c0280->layer1Map, 0);
    Bg_PutTile5Quad(26, row * 2 + 5, data_027c0280->layer1Map, 0);
  }
  Gfx_RequestLayerCopy(0);
  Gfx_RequestLayerCopy(1);
}
