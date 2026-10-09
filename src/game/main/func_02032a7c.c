#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02159d28;

MenuButton *func_02032a7c(void)
{
    return MenuButton_Add(0, 0, &data_02159d28, 0);
}
