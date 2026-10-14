#include "types.h"
#include "game/Unk_ov005_0236bba0.h"

/* The CO list cursor moves on a grid of OV005_CO_LIST_COLUMNS columns of
 * OV005_CO_LIST_ROWS rows. Sets the
 * index range the cursor can reach along each axis from its position; -1:
 * no range. */
void Ov005_GetCursorRanges(int column, int row, int* minColumn, int* maxColumn,
                           int* minRow, int* maxRow) {
  *minColumn = -1;
  *maxColumn = -1;
  *minRow = -1;
  *maxRow = -1;
  switch (column) {
    case 0:
    case 1:
    case 2:
      *minRow = 0;
      *maxRow = OV005_CO_LIST_ROWS - 1;
      break;
  }
  switch (row) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      *minColumn = 0;
      *maxColumn = OV005_CO_LIST_COLUMNS - 1;
      break;
  }
}
