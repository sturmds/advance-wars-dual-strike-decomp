#include "types.h"
#include "game/Unk_0229c734.h"

void ScoreList_DrawRow(int x, int y, int row, int tally);

#define SCORES data_0229c734

/* Draws the score rows from row firstRow on, pageRowCount of them, one under the
 * other from y + topMargin. Each row takes its height in pixels. */
void ResultPoint_DrawRows(int x, int y, int tally)
{
    int row;
    int offset = SCORES->topMargin;

    for (row = SCORES->firstRow; row < SCORES->firstRow + SCORES->pageRowCount; row++) {
        ScoreList_DrawRow(x, y + offset, row, tally);
        offset += SCORES->rows[row].height;
    }
}
