#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"

#define SE_CONFIRM 0x1000a
#define SE_CONFIRM_GREYED 0x10008

extern u8 data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);
int MapScript_RunList10(int id, void* unit);
void Sound_PlaySe(u32 soundId);

/* onConfirm of a pop-up menu button: runs map script list 10 with the item's id (unk_00); when
 * a script takes it, nothing else happens. Otherwise plays one of two sounds by MENU_ITEM_GREYED in the
 * item's flags and calls the item's handler (MenuItem.onConfirm). Returns 0 when the menu is gone
 * or a script took the item. */
BOOL PopupMenuButton_Confirm(MenuButton* button) {
  MenuItemHandler handler;
  u8 id = button->id;
  PopupMenuTask* menu = button->owner;

  if (Task_FindByScript(data_021587a8) == NULL) {
    return FALSE;
  }
  handler = menu->items[menu->itemIndex[id]].onConfirm;
  if (MapScript_RunList10(menu->items[menu->itemIndex[id]].unk_00, NULL) != 0) {
    return FALSE;
  }
  if (menu->flags[id] & MENU_ITEM_GREYED) {
    Sound_PlaySe(SE_CONFIRM_GREYED);
  } else {
    Sound_PlaySe(SE_CONFIRM);
  }
  if (handler != NULL) {
    handler(menu->itemIndex[id], id, menu->flags[id]);
  }
  return TRUE;
}
