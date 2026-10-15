#include "types.h"
#include "game/popup_menu.h"
#include "game/icon.h"

extern u8 data_0215875c[];

/* Draws the icon of each item that has one, left of the item text: the icon
 * sits one tile in from the menu corner and each item takes two tile rows. */
void PopupMenu_DrawItemIcons(PopupMenuTask* menu) {
  int i;

  for (i = 0; i < menu->count; i++) {
    if (menu->itemIcons[i] != 0xff) {
      Icon_Draw(data_0215875c[menu->itemIcons[i]], (menu->x + 1) * 8,
                (menu->y + 1 + i * 2) * 8, 0, 0);
    }
  }
}
