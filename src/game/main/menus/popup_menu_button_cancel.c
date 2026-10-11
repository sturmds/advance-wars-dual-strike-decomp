#include "types.h"
#include "game/menu_button.h"
#include "game/popup_menu.h"

#define SE_CANCEL 0x10009

extern u8 data_021587a8[];
extern u8 data_021583b4[];

PopupMenuTask *Task_FindByScript(void *script);
s8 ScriptSlot_StopByScript(const void *script);
void Sound_PlaySe(u32 soundId);

/* onCancel of a pop-up menu button: stops the text script data_021583b4 (the help text), calls
 * the item's handler (MenuItem.onCancel) with the item's index, the button id and the item's
 * flags, and plays the cancel sound. Returns 0 when the menu is gone. */
BOOL PopupMenuButton_Cancel(MenuButton *button)
{
    u8 id = button->id;
    MenuItemHandler handler;
    PopupMenuTask *menu = button->owner;

    if (Task_FindByScript(data_021587a8) == NULL) {
        return FALSE;
    }
    handler = menu->items[menu->itemIndex[id]].onCancel;
    ScriptSlot_StopByScript(data_021583b4);
    if (handler != NULL) {
        handler(menu->itemIndex[id], id, menu->flags[id]);
    }
    Sound_PlaySe(SE_CANCEL);
    return TRUE;
}
