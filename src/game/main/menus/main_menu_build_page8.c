#include "types.h"
#include "game/main_menu.h"

/* Builds main menu page 8: three entries (actions 16 to 18), none hidden. */
void MainMenu_BuildPage8(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 8;
  page->title = 8;
  page->palette = 8;
  page->entryCount = 3;

  entries[0].kind = 16;
  entries[0].palette = 8;
  entries[0].hidden = FALSE;
  entries[1].kind = 17;
  entries[1].palette = 8;
  entries[1].hidden = FALSE;
  entries[2].kind = 18;
  entries[2].palette = 8;
  entries[2].hidden = FALSE;
}
