#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/popup_menu.h"

extern Unk_027c0280 *data_027c0280;

void Bg_SetScroll(int layer, int scrollX, int scrollY);
void PopupMenu_InitIcons(PopupMenuTask *menu);
void Layer0_ClearMap(void);
void PopupMenu_SetItemIcon(int icon);
void Text_PrintImmediate(s16 x, s16 y, u16 *map, u32 textId, u16 unk_30, u16 arg);
void PopupMenu_NextIconSlot(void);
void TransferQueue_Add(const void *src, void *dst, u32 size);

#define POPUP_ICON_NONE 0xff

/* Resets layer 0's scroll and icon bank, clears the layer 0 tile map, prints the label of
 * each listed item two rows apart inside the menu box, greyed items with the last
 * Text_PrintImmediate argument set, and queues the tile map copy to VRAM. */
void PopupMenu_DrawItems(PopupMenuTask *menu)
{
    s16 i;
    s16 grey;

    Bg_SetScroll(0, 0, 0);
    PopupMenu_InitIcons(menu);
    Layer0_ClearMap();
    for (i = 0; i < menu->count; i++) {
        if (menu->flags[i] & MENU_ITEM_GREYED) {
            grey = 1;
        } else {
            grey = 0;
        }
        PopupMenu_SetItemIcon(POPUP_ICON_NONE);
        Text_PrintImmediate(menu->x + 1, menu->y + 1 + i * 2, data_027c0280->layer0Map,
                            menu->items[menu->itemIndex[i]].textId, 0, grey);
        PopupMenu_NextIconSlot();
    }
    TransferQueue_Add(data_027c0280->layer0Map, data_027c0280->unk_e8, 0x800);
}
