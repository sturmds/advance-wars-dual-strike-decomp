#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"

extern u8 data_021587a8[];

PopupMenuTask *Task_FindByScript(void *script);

/* onUpdate of a pop-up menu button: calls the item's per-frame handler (MenuItem.onUpdate)
 * with the item's index, the button id and the item's flags. */
void PopupMenuButton_Update(MenuButton *button)
{
    u8 id = button->id;
    PopupMenuTask *menu = button->owner;
    MenuItemHandler handler;

    if (Task_FindByScript(data_021587a8) == NULL) {
        return;
    }
    handler = menu->items[menu->itemIndex[id]].onUpdate;
    if (handler == NULL) {
        return;
    }
    handler(menu->itemIndex[id], id, menu->flags[id]);
}
