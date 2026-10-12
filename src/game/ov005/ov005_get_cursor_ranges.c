#include "types.h"

/* The CO list cursor moves on a grid of 3 columns of 11 rows. Sets the
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
      *maxRow = 10;
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
      *maxColumn = 2;
      break;
  }
}
