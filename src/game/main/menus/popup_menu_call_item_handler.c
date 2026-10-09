#include "types.h"
#include "game/popup_menu.h"

/* Calls the handler of the item under the cursor, if it has one. */
void PopupMenu_CallItemHandler(PopupMenuTask *menu)
{
    u8 selected = menu->selected;
    u8 index = menu->itemIndex[selected];
    const MenuItem *item = &menu->items[index];
    void (*handler)(s8, s8, s8) = item->unk_0c;

    if (handler == NULL) {
        return;
    }
    handler(index, selected, menu->flags[selected]);
}
