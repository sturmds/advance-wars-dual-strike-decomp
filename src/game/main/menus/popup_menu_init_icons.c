#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/popup_menu.h"
#include "game/icon.h"

/* Starts the pop-up menu's icon slot at its first item and empties icon
 * bank 0 for the menu's icons. */
void PopupMenu_InitIcons(PopupMenuTask* menu) {
  menu->iconSlot = 0;
  IconBank_Init(0, data_027c0280->objVram, 0x194, 16);
}
