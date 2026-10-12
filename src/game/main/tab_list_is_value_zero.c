#include "types.h"
#include "game/tab_list.h"

/* TRUE when the number shown for entry (row, col) is 0. */
BOOL TabList_IsValueZero(int row, int col) {
  return data_0215d6c4[row][col].getValue(row, col) == 0;
}
