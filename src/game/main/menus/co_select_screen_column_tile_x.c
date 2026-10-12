#include "types.h"
#include "game/co_select_screen.h"

/* First tile column of grid column `column` (counts from 1): columns are
 * 10 tiles apart, starting at 12 with one column, 7 with two, 2 with more. */
int CoSelectScreen_ColumnTileX(u8 column) {
  switch (data_0229c73c->columnCount) {
    case 1:
      return (u8)(column - 1) * 10 + 12;
    case 2:
      return (u8)(column - 1) * 10 + 7;
    default:
      return (u8)(column - 1) * 10 + 2;
  }
}
