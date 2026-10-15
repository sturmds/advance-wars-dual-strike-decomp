#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

int MainMenu_IsFirstSidePage(int page);
void MainMenu_StartPageFlip(int page, int slide);

#define SLIDE_RIGHT 3

/* Remembers the cursor, then flips to the other side page: page 6 from the
 * first side pages, page 0 from the others. */
void MainMenu_FlipToOtherSide(void) {
  MainMenu* menu = data_021599d0;

  menu->cursors[menu->page] = Menu_GetSelectedId();
  if (MainMenu_IsFirstSidePage(data_021599d0->page) == 1) {
    MainMenu_StartPageFlip(6, SLIDE_RIGHT);
  } else {
    MainMenu_StartPageFlip(0, SLIDE_RIGHT);
  }
}
