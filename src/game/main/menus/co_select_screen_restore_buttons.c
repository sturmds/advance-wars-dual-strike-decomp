#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

void CoSelectScreen_AddGridButtons(void);
void CoSelectScreen_AddCoListButtons(void);

/* Adds the buttons of whichever view is showing, the grid or the open CO
 * list, and turns touch on. */
void CoSelectScreen_RestoreButtons(void) {
  if (data_0229c73c->coListOpen == 0) {
    CoSelectScreen_AddGridButtons();
  } else {
    CoSelectScreen_AddCoListButtons();
  }
  MenuButtons_EnableTouch();
}
