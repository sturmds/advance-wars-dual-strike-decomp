#include "types.h"
#include "game/popup_menu.h"

/* also declared in src/game/main/menus/popup_menu_push_state.c and 1 more */
PopupMenuSave* data_02297ee0;
/* size: up to the next symbol; also declared in src/game/main/menus/popup_menu_reset_stack.c */
u8 data_02297ee4[0x40];

void PopupMenu_Create(const MenuItem* items, int x, int y, u16 style,
                      u16 selected, int unused);

/* Takes the last entry off the stack that PopupMenu_PushState fills and opens
 * that pop-up menu again. Does nothing when the stack is empty. The (s8) cast is
 * needed to match: the original loads the cursor as signed. */
void PopupMenu_PopState(void) {
  PopupMenuSave* save;

  if ((u8*)data_02297ee0 == data_02297ee4) {
    return;
  }
  save = --data_02297ee0;
  PopupMenu_Create(save->items, save->x, save->y, save->style,
                   (s8)save->selected, 0);
}
