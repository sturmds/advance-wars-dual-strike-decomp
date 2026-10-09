#include "types.h"
#include "game/menu_button.h"

MenuButton *func_ov000_022b0680(int id);
void func_ov000_022b04ec(MenuButton *button);

void func_ov000_022b0500(int id)
{
    MenuButton *button = func_ov000_022b0680(id);

    if (button == NULL) {
        return;
    }
    func_ov000_022b04ec(button);
}
