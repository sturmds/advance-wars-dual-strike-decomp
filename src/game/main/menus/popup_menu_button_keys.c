#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"
#include "game/task.h"

extern TaskScriptCmd data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);
void PopupMenu_HandleKeys(PopupMenuTask* menu, MenuButton* button);

/* onKeys of a pop-up menu button: passes the key handling to PopupMenu_HandleKeys while the
 * menu task exists. */
void PopupMenuButton_Keys(MenuButton* button) {
  PopupMenuTask* menu = button->owner;

  if (Task_FindByScript(data_021587a8) == NULL) {
    return;
  }
  PopupMenu_HandleKeys(menu, button);
}
