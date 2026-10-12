#include "types.h"
#include "game/menu_button.h"

/* Removes the active button with this id at once, without the closing
 * animation. */
void MenuButton_Remove(int id) {
  MenuButton* button = MenuButton_Find(id);

  if (button == NULL) {
    return;
  }
  MenuButton_Deactivate(button);
}
