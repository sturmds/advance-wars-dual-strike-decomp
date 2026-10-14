#include "types.h"
#include "game/popup_menu.h"
#include "game/task.h"

extern TaskScriptCmd data_021587a8[];
extern u8 data_0215875c[];

PopupMenuTask* Task_FindByScript(void* script);
void Icon_Load(int iconId);

/* Sets the icon of the pop-up menu item at the current icon slot and loads
 * it. icon is an index into data_0215875c, or 0xff for no icon. */
void PopupMenu_SetItemIcon(int icon) {
  PopupMenuTask* menu = Task_FindByScript(data_021587a8);

  if (menu == NULL) {
    return;
  }
  menu->itemIcons[menu->iconSlot] = icon;
  if (icon == 0xff) {
    return;
  }
  Icon_Load(data_0215875c[icon]);
}
