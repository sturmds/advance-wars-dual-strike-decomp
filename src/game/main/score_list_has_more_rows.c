#include "types.h"
#include "game/Unk_0229c734.h"

/* TRUE when the row right after the current page is in use (kind not 0). */
BOOL ScoreList_HasMoreRows(void) {
  return data_0229c734
             ->rows[data_0229c734->firstRow + data_0229c734->pageRowCount]
             .kind != 0;
}
