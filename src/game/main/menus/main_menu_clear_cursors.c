#include "types.h"
#include "game/main_menu.h"

/* Puts the cursor of every page back on the first entry. */
void MainMenu_ClearCursors(void) {
  int i;

  data_021599d0->unk_11 &= ~1;
  for (i = 0; i < (int)ARRAY_COUNT(data_021599d0->cursors); i++) {
    data_021599d0->cursors[i] = 0;
  }
}
