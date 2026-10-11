#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 8: three entries (actions 16 to 18), none hidden. */
void MainMenu_BuildPage8(void)
{
    MainMenuPage *page = *data_0229c71c;
    MainMenuEntry *entries = page->entries;

    data_021599d0->page = 8;
    page->unk_00 = 8;
    page->unk_01 = 8;
    page->entryCount = 3;

    entries[0].unk_00 = 16;
    entries[0].unk_01 = 8;
    entries[0].hidden = FALSE;
    entries[1].unk_00 = 17;
    entries[1].unk_01 = 8;
    entries[1].hidden = FALSE;
    entries[2].unk_00 = 18;
    entries[2].unk_01 = 8;
    entries[2].hidden = FALSE;
}
