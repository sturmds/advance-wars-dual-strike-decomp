#include "types.h"
#include "game/main_menu.h"

void func_020308c8(void)
{
    MainMenuPage *page = *data_0229c71c;
    MainMenuEntry *entries = page->entries;

    data_021599d0->page = 12;
    page->unk_00 = 5;
    page->unk_01 = 3;
    page->entryCount = 3;

    entries[0].unk_00 = 25;
    entries[0].unk_01 = 3;
    entries[0].hidden = FALSE;
    entries[1].unk_00 = 26;
    entries[1].unk_01 = 3;
    entries[1].hidden = FALSE;
    entries[2].unk_00 = 27;
    entries[2].unk_01 = 3;
    entries[2].hidden = FALSE;
}
