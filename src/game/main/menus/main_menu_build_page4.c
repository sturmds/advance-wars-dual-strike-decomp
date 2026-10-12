#include "types.h"
#include "game/main_menu.h"

s8 func_02102394(void);

/* Builds main menu page 4: two entries (actions 11 and 12). Entry 1 is
 * hidden while func_02102394 returns 0. */
void MainMenu_BuildPage4(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 4;
  page->title = 4;
  page->palette = 3;
  page->entryCount = 2;

  entries[0].kind = 11;
  entries[0].palette = 3;
  entries[0].hidden = FALSE;
  entries[1].kind = 12;
  entries[1].palette = 3;
  if (func_02102394()) {
    entries[1].hidden = FALSE;
  } else {
    entries[1].hidden = TRUE;
  }
}
