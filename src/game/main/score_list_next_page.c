#include "types.h"
#include "game/Unk_0229c734.h"

void ScoreList_UpdatePageMax(void);

#define SCORE_LIST_PAGE_HEIGHT 144

/* Moves to the next page of the score list: unk_9c becomes the first row
 * after the old page, unk_9d the number of rows that fit in 144 pixels, and
 * unk_9e the top margin that centres them. */
void ScoreList_NextPage(void)
{
    int count;
    int i;
    int usedHeight;

    count = usedHeight = 0;

    data_0229c734->unk_9c += data_0229c734->unk_9d;
    for (i = data_0229c734->unk_9c; i < SCORE_ROW_COUNT; i++) {
        if (data_0229c734->rows[i].kind == 0) {
            break;
        }
        if (usedHeight + data_0229c734->rows[i].height > SCORE_LIST_PAGE_HEIGHT) {
            break;
        }
        usedHeight += data_0229c734->rows[i].height;
        count++;
    }
    data_0229c734->unk_9e = (SCORE_LIST_PAGE_HEIGHT - usedHeight) >> 1;
    data_0229c734->unk_9d = count;
    ScoreList_UpdatePageMax();
}
