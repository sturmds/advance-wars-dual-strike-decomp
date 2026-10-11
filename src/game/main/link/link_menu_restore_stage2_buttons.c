#include "types.h"
#include "game/Unk_0229fb30.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02160150;

int func_0207a260(void);
int func_0207fb98(int player, int index);

/* Rebuilds the stage 2 buttons (ids 2-4, a column per link player) and selects the one this
 * side picked before. The picking side gets only the buttons func_0207fb98 allows. */
void LinkMenu_RestoreStage2Buttons(void)
{
    int i;

    if (func_0207a260() != 0) {
        MenuButton_Close(254);
    }
    for (i = 0; i < 3; i++) {
        if (data_0229fb30.unk_631 != func_0207a260() || func_0207fb98(func_0207a260(), i) != 0) {
            MenuButton_Add(func_0207a260() * 96 + 40, i * 16 + 120, &data_02160150, i + 2);
        }
    }
    if (data_0229fb30.unk_631 == func_0207a260()) {
        MenuButton_Select(data_0229fb30.unk_632 + 2);
    } else {
        MenuButton_Select(data_0229fb30.unk_633 + 2);
    }
}
