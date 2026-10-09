#include "types.h"
#include "game/menu_button.h"

void func_0205dea0(void);

void MapSelect_OnSlideDone(void)
{
    func_0205dea0();
    MenuButton_EnableTouch(254);
    MenuButton_EnableTouch(252);
    MenuButton_EnableTouch(253);
    MenuButton_EnableTouch(251);
    MenuButton_EnableTouch(250);
}
