#include "types.h"
#include "game/Unk_0229c734.h"

/* Counts the rows on the current page of the score list (firstRow is the first
 * row, pageRowCount the number of rows) that give at least this many points. */
int ScoreList_CountRowsAtLeast(int points) {
  int i;
  int count = 0;

  for (i = data_0229c734->firstRow;
       i < data_0229c734->firstRow + data_0229c734->pageRowCount; i++) {
    if (points <= data_0229c734->rows[i].points) {
      count++;
    }
  }
  return count;
}
