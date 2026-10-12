#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 0, the first side page: five entries (actions 0 to
 * 4), none hidden. */
void MainMenu_BuildPage0(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 0;
  page->title = 0;
  page->palette = 0;
  page->entryCount = 5;

  entries[0].kind = 0;
  entries[0].palette = 1;
  entries[0].hidden = FALSE;
  entries[1].kind = 1;
  entries[1].palette = 2;
  entries[1].hidden = FALSE;
  entries[2].kind = 2;
  entries[2].palette = 0;
  entries[2].hidden = FALSE;
  entries[3].kind = 3;
  entries[3].palette = 3;
  entries[3].hidden = FALSE;
  entries[4].kind = 4;
  entries[4].palette = 3;
  entries[4].hidden = FALSE;
}
