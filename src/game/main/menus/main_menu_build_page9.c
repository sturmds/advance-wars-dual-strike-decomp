#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 9: three entries (actions 13 to 15), none hidden. */
void MainMenu_BuildPage9(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 9;
  page->title = 4;
  page->palette = 3;
  page->entryCount = 3;

  entries[0].kind = 13;
  entries[0].palette = 3;
  entries[0].hidden = FALSE;
  entries[1].kind = 14;
  entries[1].palette = 3;
  entries[1].hidden = FALSE;
  entries[2].kind = 15;
  entries[2].palette = 3;
  entries[2].hidden = FALSE;
}
