#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"
#include "game/task.h"

#define SE_MOVE 0x10002

extern TaskScriptCmd data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);
void Sound_PlaySe(u32 soundId);

/* onSelect of a pop-up menu button (the cursor moves onto it): plays the cursor move sound
 * and calls the item's handler (MenuItem.onSelect) with the item's index, the button id and
 * the item's flags. */
void PopupMenuButton_Select(MenuButton* button) {
  u8 id = button->id;
  PopupMenuTask* menu = button->owner;
  MenuItemHandler handler;

  if (Task_FindByScript(data_021587a8) == NULL) {
    return;
  }
  Sound_PlaySe(SE_MOVE);
  handler = menu->items[menu->itemIndex[id]].onSelect;
  if (handler == NULL) {
    return;
  }
  handler(menu->itemIndex[id], id, menu->flags[id]);
}
