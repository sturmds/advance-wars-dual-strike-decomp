#include "types.h"
#include "game/tab_list.h"

/* Returns what the entry's unk_1c function returns, or 0 when it has none. */
int TabList_CallUnk1c(int row, int col) {
  if (data_0215d6c4[row][col].unk_1c == NULL) {
    return 0;
  }
  return data_0215d6c4[row][col].unk_1c(row, col);
}
