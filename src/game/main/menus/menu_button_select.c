#include "types.h"
#include "game/menu_button.h"

/* Puts the menu cursor on the active button with this id. The id is kept
 * even when no button has it. */
MenuButton* MenuButton_Select(int id) {
  return MenuCursor_SetSelected(MenuButton_Find(id), id);
}
