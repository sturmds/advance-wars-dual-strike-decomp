#include "types.h"
#include "game/popup_menu.h"
#include "game/task.h"

extern const TaskScriptCmd data_021587a8[];

void NumberFont_LoadLow(void);
const void *Text_Get(u32 textId);
int Text_GetWidthInTiles(const void *text);
void PopupMenu_DrawItems(PopupMenuTask *menu);
void PopupMenu_DestroyStyle1(Task *task);
void PopupMenu_DestroyStyle0(Task *task);
void MenuBox_Draw(int x, int y, int width, int height);

/* Creates a pop-up menu task from the item table items and draws its box.
 * Items whose getFlags result has MENU_ITEM_HIDDEN set are left out. The box
 * is the widest label plus 2 tiles wide and 2 tiles per item (plus 2) high.
 * x and y are the top-left tile (with center set, x is the middle) and are
 * moved to keep the box on the 32 by 24 tile screen. selected is the first
 * cursor row and is capped to the last item. style 0 or 1 picks onDestroy. */
PopupMenuTask *PopupMenu_Create(const MenuItem *items, int x, int y, int style, u16 selected, int center)
{
    s16 listedCount;
    PopupMenuTask *menu;
    const MenuItem *item;
    int boxWidth;
    u32 heightPixels;
    int i;
    u16 labelWidth;
    int height;
    u32 maxWidth;
    Task *task;
    listedCount = 0;
    maxWidth = 0;
    heightPixels = 16;

    NumberFont_LoadLow();
    task = Task_Create(data_021587a8, TASK_LIST(3));
    menu = (PopupMenuTask *)task;
    menu->style = style;
    switch (style) {
    case 0:
        task->onDestroy = PopupMenu_DestroyStyle0;
        break;
    case 1:
        task->onDestroy = PopupMenu_DestroyStyle1;
        break;
    }
    menu->items = items;
    for (i = 0;; i++) {
        item = &menu->items[i];
        if (item->unk_00 == MENU_ITEM_END) {
            break;
        }
        menu->flags[listedCount] = item->getFlags();
        if (!(menu->flags[listedCount] & MENU_ITEM_HIDDEN)) {
            menu->itemIndex[listedCount++] = i;
            labelWidth = Text_GetWidthInTiles(Text_Get(item->textId));
            if (maxWidth < labelWidth) {
                maxWidth = labelWidth;
            }
            heightPixels = (u16)(heightPixels + 16);
        }
    }
    menu->itemTotal = i;
    menu->count = listedCount;
    boxWidth = maxWidth + 2;
    if (menu->count - 1 < selected) {
        selected = menu->count - 1;
    }
    if (center != 0) {
        x -= maxWidth >> 1;
    }
    menu->selected = selected;
    if (x + boxWidth >= 32) {
        x = 32 - boxWidth;
    }
    height = heightPixels >> 3;
    if (x < 0) {
        x = 0;
    }
    menu->width = maxWidth;
    if (y + (int)(heightPixels / 8) >= 24) {
        y = 24 - height;
    }
    if (y < 0) {
        y = 0;
    }
    menu->x = x;
    menu->y = y;
    PopupMenu_DrawItems(menu);
    MenuBox_Draw(menu->x, menu->y, boxWidth, height);
    return menu;
}
