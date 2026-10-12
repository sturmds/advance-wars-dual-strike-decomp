#include "types.h"
#include "game/build_menu.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate gBuildMenuRowButton;

void MenuBox_Draw(int x, int y, int width, int height);
void BuildMenu_DrawColumn(int x, int y, int first);

/* Draws one column of the build menu: a 16 by 16 tile box at tile (x, y), the
 * rows of gBuildMenuEntries from `first` in it, and a touch button over each
 * row, with the entry index as its id. */
void BuildMenu_DrawColumnWithButtons(int x, int y, int first) {
  int row;
  int buttonY;
  int buttonX;

  MenuBox_Draw(x, y, 16, 16);
  BuildMenu_DrawColumn(x, y, first);
  buttonY = (y + 1) * 8;
  buttonX = (x + 1) * 8;
  for (row = 0; row < BUILD_MENU_ROWS; row++) {
    if (gBuildMenuEntries[first + row].unitType == 0) {
      return;
    }
    MenuButton_Add(buttonX, buttonY, &gBuildMenuRowButton, first + row);
    buttonY += 16;
  }
}
