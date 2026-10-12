#include "types.h"
#include "game/main_menu.h"

s8 func_021023b4(void);

/* Builds main menu page 3: two entries (actions 11 and 12). Entry 1 is
 * hidden while func_021023b4 returns 0. */
void MainMenu_BuildPage3(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 3;
  page->title = 3;
  page->palette = 0;
  page->entryCount = 2;

  entries[0].kind = 11;
  entries[0].palette = 0;
  entries[0].hidden = FALSE;
  entries[1].kind = 12;
  entries[1].palette = 0;
  if (func_021023b4()) {
    entries[1].hidden = FALSE;
  } else {
    entries[1].hidden = TRUE;
  }
}
