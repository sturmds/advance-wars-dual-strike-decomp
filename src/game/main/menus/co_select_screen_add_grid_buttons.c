#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate gCoGridButtonTemplate;

int CoSelectScreen_ColumnTileX(u8 column);
int CoSelectScreen_RowTileY(u8 row);

/* Selects the button of the cursor cell, then adds one button per grid
 * cell. A cell's id is (column - 1) * 2 + row. */
void CoSelectScreen_AddGridButtons(void) {
  int column;
  int row;

  MenuButton_Select(((data_0229c73c->cursorColumn - 1) << 1) +
                    data_0229c73c->cursorRow);
  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    for (row = 0; row < data_0229c73c->rowCount; row++) {
      MenuButton_Add((CoSelectScreen_ColumnTileX(column) + 1) * 8,
                     (CoSelectScreen_RowTileY(row) + 2) * 8,
                     &gCoGridButtonTemplate, (column - 1) * 2 + row);
    }
  }
}
