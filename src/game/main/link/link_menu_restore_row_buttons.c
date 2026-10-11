#include "types.h"
#include "game/menu_button.h"
#include "game/link_menu_state.h"

extern const MenuButtonTemplate data_0215fefc;
extern const MenuButtonTemplate data_0215ff24;

int func_0207a27c(void);

/* In link states 2 and 3, adds the row buttons again when button 0 is gone,
 * and selects the remembered button. */
void LinkMenu_RestoreRowButtons(void)
{
    int i;

    if (func_0207a27c() != 3 && func_0207a27c() != 2) {
        return;
    }
    if (MenuButton_Find(0) != NULL) {
        return;
    }
    for (i = 0; i < 5; i++) {
        if (i == 4) {
            MenuButton_Add(0, i * 40, &data_0215ff24, i);
        } else {
            MenuButton_Add(0, i * 40, &data_0215fefc, i);
        }
    }
    MenuButton_Select(data_0215fed0->selected);
}
