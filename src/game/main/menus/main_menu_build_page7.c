#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 7: six entries (actions 19 to 24), none hidden. */
void MainMenu_BuildPage7(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 7;
  page->title = 7;
  page->palette = 4;
  page->entryCount = 6;

  entries[0].kind = 19;
  entries[0].palette = 4;
  entries[0].hidden = FALSE;
  entries[1].kind = 20;
  entries[1].palette = 4;
  entries[1].hidden = FALSE;
  entries[2].kind = 21;
  entries[2].palette = 4;
  entries[2].hidden = FALSE;
  entries[3].kind = 22;
  entries[3].palette = 4;
  entries[3].hidden = FALSE;
  entries[4].kind = 23;
  entries[4].palette = 4;
  entries[4].hidden = FALSE;
  entries[5].kind = 24;
  entries[5].palette = 4;
  entries[5].hidden = FALSE;
}
