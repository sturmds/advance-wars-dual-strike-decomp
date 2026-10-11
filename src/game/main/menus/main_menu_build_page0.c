#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 0, the first side page: five entries (actions 0 to
 * 4), none hidden. */
void MainMenu_BuildPage0(void)
{
    MainMenuPage *page = *data_0229c71c;
    MainMenuEntry *entries = page->entries;

    data_021599d0->page = 0;
    page->unk_00 = 0;
    page->unk_01 = 0;
    page->entryCount = 5;

    entries[0].unk_00 = 0;
    entries[0].unk_01 = 1;
    entries[0].hidden = FALSE;
    entries[1].unk_00 = 1;
    entries[1].unk_01 = 2;
    entries[1].hidden = FALSE;
    entries[2].unk_00 = 2;
    entries[2].unk_01 = 0;
    entries[2].hidden = FALSE;
    entries[3].unk_00 = 3;
    entries[3].unk_01 = 3;
    entries[3].hidden = FALSE;
    entries[4].unk_00 = 4;
    entries[4].unk_01 = 3;
    entries[4].hidden = FALSE;
}
