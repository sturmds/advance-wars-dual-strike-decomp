#include "types.h"
#include "game/menu_button.h"

void func_02025ce8(MenuButton *button);

/* Removes the active button with this id at once, without the closing
 * animation. */
void MenuButton_Remove(int id)
{
    MenuButton *button = MenuButton_Find(id);

    if (button == NULL) {
        return;
    }
    func_02025ce8(button);
}
