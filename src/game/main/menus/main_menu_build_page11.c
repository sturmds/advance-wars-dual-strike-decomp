#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 11: two entries (actions 25 and 26), none hidden. */
void MainMenu_BuildPage11(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 11;
  page->title = 1;
  page->palette = 1;
  page->entryCount = 2;

  entries[0].kind = 25;
  entries[0].palette = 1;
  entries[0].hidden = FALSE;
  entries[1].kind = 26;
  entries[1].palette = 1;
  entries[1].hidden = FALSE;
}
