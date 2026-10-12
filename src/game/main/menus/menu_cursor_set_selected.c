#include "types.h"
#include "game/menu_button.h"

/* Puts the cursor on a button. When the cursor moves off a selectable
 * button to a different one, MenuButton_NotifySelected runs the new button's
 * callback at 0x34. */
MenuButton* MenuCursor_SetSelected(MenuButton* button, int id) {
  BOOL changed;

  if (MenuCursor_HasSelection() && button != NULL &&
      gMenuCursor->selected != button) {
    changed = TRUE;
  } else {
    changed = FALSE;
  }
  gMenuCursor->selected = button;
  gMenuCursor->selectedId = id;
  if (changed) {
    MenuButton_NotifySelected(gMenuCursor->selected);
  }
  return gMenuCursor->selected;
}
