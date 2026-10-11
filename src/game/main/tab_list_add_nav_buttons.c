#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215d4ec;
extern const MenuButtonTemplate data_0215d514;
extern const MenuButtonTemplate data_0215d564;
extern const MenuButtonTemplate data_0215d58c;
extern const MenuButtonTemplate data_0215d5b4;
extern const MenuButtonTemplate data_0215d5dc;
extern const MenuButtonTemplate data_0215d604;

void TabList_AddNavButtons(void)
{
    MenuButton_Add(0, 0, &data_0215d564, 254);
    MenuButton_Add(0, 0, &data_0215d514, 253);
    MenuButton_Add(0, 0, &data_0215d58c, 252);
    MenuButton_Add(0, 0, &data_0215d604, 251);
    MenuButton_Add(0, 0, &data_0215d5b4, 250);
    MenuButton_Add(0, 0, &data_0215d5dc, 249);
    MenuButton_Add(0, 0, &data_0215d4ec, 248);
}
