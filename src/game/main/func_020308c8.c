#include "types.h"
#include "game/main_menu.h"

void func_020308c8(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 12;
  page->title = 5;
  page->palette = 3;
  page->entryCount = 3;

  entries[0].kind = 25;
  entries[0].palette = 3;
  entries[0].hidden = FALSE;
  entries[1].kind = 26;
  entries[1].palette = 3;
  entries[1].hidden = FALSE;
  entries[2].kind = 27;
  entries[2].palette = 3;
  entries[2].hidden = FALSE;
}
