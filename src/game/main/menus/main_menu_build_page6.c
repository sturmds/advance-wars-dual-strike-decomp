#include "types.h"
#include "game/main_menu.h"
#include "game/unlocks.h"

/* Builds main menu page 6, the second side page: six entries (actions 5 to
 * 10). Entry 3 stays hidden until game flag 38 is set, entry 4 until flag 37. */
void MainMenu_BuildPage6(void)
{
    MainMenuPage *page = *data_0229c71c;
    MainMenuEntry *entries = page->entries;

    data_021599d0->page = 6;
    page->unk_00 = 6;
    page->unk_01 = 4;
    page->entryCount = 6;

    entries[0].unk_00 = 5;
    entries[0].unk_01 = 5;
    entries[0].hidden = FALSE;
    entries[1].unk_00 = 6;
    entries[1].unk_01 = 7;
    entries[1].hidden = FALSE;
    entries[2].unk_00 = 7;
    entries[2].unk_01 = 4;
    entries[2].hidden = FALSE;
    entries[3].unk_00 = 8;
    entries[3].unk_01 = 6;
    if (GameFlag_IsSet(38)) {
        entries[3].hidden = FALSE;
    } else {
        entries[3].hidden = TRUE;
    }
    entries[4].unk_00 = 9;
    entries[4].unk_01 = 6;
    if (GameFlag_IsSet(37)) {
        entries[4].hidden = FALSE;
    } else {
        entries[4].hidden = TRUE;
    }
    entries[5].unk_00 = 10;
    entries[5].unk_01 = 8;
    entries[5].hidden = FALSE;
}
