#include "types.h"
#include "game/main_menu.h"
#include "game/play_records.h"

int func_021023c4(void);
s8 func_021023d4(void);
s8 func_021023e4(void);

/* Builds main menu page 10: three entries (actions 31, 32 and 33), each
 * hidden while its unlock check (func_021023e4, func_021023d4,
 * func_021023c4) returns 0. The cursor goes to the entry saved in
 * gPlayRecords.unk_11 when that entry is unlocked. */
void MainMenu_BuildPage10(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entries = page->entries;

  data_021599d0->page = 10;

  if (gPlayRecords.unk_11 == 0 && func_021023e4()) {
    data_021599d0->cursors[data_021599d0->page] = 0;
  }
  if (gPlayRecords.unk_11 == 1 && func_021023d4()) {
    data_021599d0->cursors[data_021599d0->page] = 1;
  }
  if (gPlayRecords.unk_11 == 2 && func_021023c4()) {
    data_021599d0->cursors[data_021599d0->page] = 2;
  }

  page->title = 1;
  page->palette = 1;
  page->entryCount = 3;

  entries[0].kind = 31;
  entries[0].palette = 1;
  if (func_021023e4()) {
    entries[0].hidden = FALSE;
  } else {
    entries[0].hidden = TRUE;
  }
  entries[1].kind = 32;
  entries[1].palette = 1;
  if (func_021023d4()) {
    entries[1].hidden = FALSE;
  } else {
    entries[1].hidden = TRUE;
  }
  entries[2].kind = 33;
  entries[2].palette = 1;
  if (func_021023c4()) {
    entries[2].hidden = FALSE;
  } else {
    entries[2].hidden = TRUE;
  }
}
