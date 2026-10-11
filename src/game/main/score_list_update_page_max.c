#include "types.h"
#include "game/Unk_0229c734.h"

/* Stores the largest points value on the current page of the score list in
 * pageMaxPoints (rows firstRow up to firstRow + pageRowCount). */
void ScoreList_UpdatePageMax(void)
{
    int i;

    data_0229c734->pageMaxPoints = 0;
    for (i = data_0229c734->firstRow; i < data_0229c734->firstRow + data_0229c734->pageRowCount; i++) {
        if (data_0229c734->pageMaxPoints < data_0229c734->rows[i].points) {
            data_0229c734->pageMaxPoints = data_0229c734->rows[i].points;
        }
    }
}
