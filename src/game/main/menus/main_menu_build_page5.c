#include "types.h"
#include "game/main_menu.h"

int func_02102378(void);

/* Builds main menu page 5: three entries (actions 11, 12 and 28). Entry 1
 * is hidden while func_02102378 returns 0. */
void MainMenu_BuildPage5(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 5;
  page->title = 5;
  page->palette = 3;
  page->entryCount = 3;

  entries[0].kind = 11;
  entries[0].palette = 3;
  entries[0].hidden = FALSE;
  entries[1].kind = 12;
  entries[1].palette = 3;
  if (func_02102378()) {
    entries[1].hidden = FALSE;
  } else {
    entries[1].hidden = TRUE;
  }
  entries[2].kind = 28;
  entries[2].palette = 3;
  entries[2].hidden = FALSE;
}
