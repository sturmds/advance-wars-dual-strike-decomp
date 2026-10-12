#include "types.h"
#include "game/main_menu.h"

int func_02102378(void);

/* Makes page 5 the page the main menu opens on (also its previous page).
 * The cursor goes on entry 1 when func_02102378 returns 1 (the page's hidden
 * entry is shown), else on entry 0. */
void MainMenu_ReturnToPage5(void) {
  data_021599d0->page = 5;
  data_021599d0->prevPage = data_021599d0->page;
  if (func_02102378()) {
    data_021599d0->cursors[data_021599d0->page] = 1;
  } else {
    data_021599d0->cursors[data_021599d0->page] = 0;
  }
}
