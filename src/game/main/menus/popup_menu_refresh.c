#include "types.h"
#include "game/popup_menu.h"

extern u8 data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);
const void* Text_Get(u32 textId);
int Text_GetWidthInTiles(const void* text);
void PopupMenu_DrawItems(PopupMenuTask* menu);
void MenuBox_Draw(int x, int y, int width, int height);

/* Lists the items of the open pop-up menu again, as PopupMenu_Create does,
 * and redraws the menu at the same place. The box is not moved or resized
 * to stay on the screen, and the cursor is left alone. */
void PopupMenu_Refresh(void) {
  s16 listedCount;
  u32 heightPixels;
  u32 maxWidth;
  int i;
  u16 labelWidth;
  PopupMenuTask* menu;
  const MenuItem* item;

  listedCount = 0;
  maxWidth = 0;
  heightPixels = 16;
  menu = Task_FindByScript(data_021587a8);
  for (i = 0;; i++) {
    item = &menu->items[i];
    if (item->unk_00 == MENU_ITEM_END) {
      break;
    }
    menu->flags[listedCount] = item->getFlags();
    if (!(menu->flags[listedCount] & MENU_ITEM_HIDDEN)) {
      menu->itemIndex[listedCount++] = i;
      labelWidth = Text_GetWidthInTiles(Text_Get(item->textId));
      if (maxWidth < labelWidth) {
        maxWidth = labelWidth;
      }
      heightPixels = (u16)(heightPixels + 16);
    }
  }
  menu->itemTotal = i;
  menu->count = listedCount;
  PopupMenu_DrawItems(menu);
  MenuBox_Draw(menu->x, menu->y, maxWidth + 2, heightPixels >> 3);
}
