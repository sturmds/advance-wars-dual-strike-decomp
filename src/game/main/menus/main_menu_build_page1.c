#include "types.h"
#include "game/main_menu.h"

s8 func_021023e4(void);
s8 func_021023d4(void);
int func_021023c4(void);

/* Builds main menu page 1: two entries (actions 11 and 12). Entry 1 is
 * hidden unless one of func_021023e4, func_021023d4 or func_021023c4
 * returns nonzero. */
void MainMenu_BuildPage1(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 1;
  page->title = 1;
  page->palette = 1;
  page->entryCount = 2;

  entries[0].kind = 11;
  entries[0].palette = 1;
  entries[0].hidden = FALSE;
  entries[1].kind = 12;
  entries[1].palette = 1;
  if (func_021023e4() || func_021023d4() || func_021023c4()) {
    entries[1].hidden = FALSE;
  } else {
    entries[1].hidden = TRUE;
  }
}
