#include "types.h"
#include "game/Unk_0229c734.h"

void ScoreList_UpdatePageMax(void);

#define SCORE_LIST_PAGE_HEIGHT 144

/* Moves to the next page of the score list: firstRow becomes the first row
 * after the old page, pageRowCount the number of rows that fit in 144 pixels, and
 * topMargin the top margin that centres them. */
void ScoreList_NextPage(void)
{
    int count;
    int i;
    int usedHeight;

    count = usedHeight = 0;

    data_0229c734->firstRow += data_0229c734->pageRowCount;
    for (i = data_0229c734->firstRow; i < SCORE_ROW_COUNT; i++) {
        if (data_0229c734->rows[i].kind == 0) {
            break;
        }
        if (usedHeight + data_0229c734->rows[i].height > SCORE_LIST_PAGE_HEIGHT) {
            break;
        }
        usedHeight += data_0229c734->rows[i].height;
        count++;
    }
    data_0229c734->topMargin = (SCORE_LIST_PAGE_HEIGHT - usedHeight) >> 1;
    data_0229c734->pageRowCount = count;
    ScoreList_UpdatePageMax();
}
