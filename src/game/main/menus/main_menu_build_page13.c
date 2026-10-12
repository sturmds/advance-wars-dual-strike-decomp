#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 13: two entries (actions 29 and 30), none hidden. */
void MainMenu_BuildPage13(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 13;
  page->title = 3;
  page->palette = 0;
  page->entryCount = 2;

  entries[0].kind = 29;
  entries[0].palette = 0;
  entries[0].hidden = FALSE;
  entries[1].kind = 30;
  entries[1].palette = 0;
  entries[1].hidden = FALSE;
}
