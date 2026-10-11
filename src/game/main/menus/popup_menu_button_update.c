#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"

extern u8 data_021587a8[];

PopupMenuTask *Task_FindByScript(void *script);

/* onUpdate of a pop-up menu button: calls the item's per-frame handler (MenuItem.unk_08)
 * with the item's index, the button id and the item's flags. */
void PopupMenuButton_Update(MenuButton *button)
{
    u8 id = button->id;
    PopupMenuTask *menu = button->owner;
    void (*handler)(s8, s8, s8);

    if (Task_FindByScript(data_021587a8) == NULL) {
        return;
    }
    handler = (void (*)(s8, s8, s8))menu->items[menu->itemIndex[id]].unk_08;
    if (handler == NULL) {
        return;
    }
    handler(menu->itemIndex[id], id, menu->flags[id]);
}
