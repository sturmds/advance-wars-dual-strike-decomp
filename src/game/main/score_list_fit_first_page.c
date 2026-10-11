#include "types.h"
#include "game/Unk_0229c734.h"

void Mem_Free(void *block);
void ScoreList_UpdatePageMax(void);

#define SCORE_LIST_PAGE_HEIGHT 144

/* Frees the loaded result graphics file, cuts the first page of the score
 * list (pageRowCount rows) down to the rows that fit in 144 pixels, centres them
 * and finds the page maximum. */
void ScoreList_FitFirstPage(void)
{
    int usedHeight = 0;
    int i;

    Mem_Free(data_0229c734->gfxFile);
    for (i = data_0229c734->firstRow; i < data_0229c734->pageRowCount; i++) {
        int next = usedHeight + data_0229c734->rows[i].height;

        if (next <= SCORE_LIST_PAGE_HEIGHT) {
            usedHeight = next;
        } else {
            data_0229c734->pageRowCount = i;
            break;
        }
    }
    data_0229c734->topMargin = (SCORE_LIST_PAGE_HEIGHT - usedHeight) >> 1;
    ScoreList_UpdatePageMax();
}
