#include "types.h"
#include "game/popup_menu.h"

extern u8 data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);
void PopupMenu_DrawItemIcons(PopupMenuTask* menu);

/* Draws the item icons of the pop-up menu, as long as the menu task exists. */
void PopupMenu_Draw(PopupMenuTask* menu) {
  if (Task_FindByScript(data_021587a8) == NULL) {
    return;
  }
  PopupMenu_DrawItemIcons(menu);
}
