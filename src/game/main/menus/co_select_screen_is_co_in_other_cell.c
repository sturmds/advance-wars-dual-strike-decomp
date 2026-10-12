#include "types.h"
#include "game/co_select_screen.h"

/* TRUE when `co` sits in a grid cell other than the cursor cell. */
BOOL CoSelectScreen_IsCoInOtherCell(u8 co) {
  int column;
  int row;

  if (co == 0) {
    return FALSE;
  }
  for (column = 1; column < data_0229c73c->columnCount + 1; column++) {
    for (row = 0; row < data_0229c73c->rowCount; row++) {
      if (column == data_0229c73c->cursorColumn &&
          row == data_0229c73c->cursorRow) {
        continue;
      }
      if (co == (&data_0229c73c->coGrid[(column - 1) * 2])[row]) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
