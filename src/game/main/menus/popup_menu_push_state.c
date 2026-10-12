#include "types.h"
#include "game/popup_menu.h"

extern PopupMenuSave* data_02297ee0;
extern u8 data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);

/* Saves the open pop-up menu (items, cursor, position, style) on a stack so
 * it can be recreated after another menu closes. Does nothing when no
 * pop-up menu task exists. */
void PopupMenu_PushState(void) {
  PopupMenuTask* menu = Task_FindByScript(data_021587a8);

  if (menu == NULL) {
    return;
  }
  data_02297ee0->items = menu->items;
  data_02297ee0->selected = menu->selected;
  data_02297ee0->x = menu->x;
  data_02297ee0->y = menu->y;
  data_02297ee0->style = menu->style;
  data_02297ee0++;
}
