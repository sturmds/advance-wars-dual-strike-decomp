#include "types.h"
#include "game/popup_menu.h"
#include "game/task.h"

extern TaskScriptCmd data_021587a8[];

PopupMenuTask* Task_FindByScript(void* script);

/* Moves the pop-up menu's icon slot on to the next item. */
void PopupMenu_NextIconSlot(void) {
  PopupMenuTask* menu = Task_FindByScript(data_021587a8);

  if (menu != NULL) {
    menu->iconSlot++;
  }
}
