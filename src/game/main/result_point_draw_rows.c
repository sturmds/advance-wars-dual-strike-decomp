#include "types.h"
#include "game/Unk_0229c734.h"

void ScoreList_DrawRow(int x, int y, int row, int tally);

#define SCORES data_0229c734

/* Draws the score rows from row unk_9c on, unk_9d of them, one under the
 * other from y + unk_9e. Each row takes its height in pixels. */
void func_020388dc(int x, int y, int tally)
{
    int row;
    int offset = SCORES->unk_9e;

    for (row = SCORES->unk_9c; row < SCORES->unk_9c + SCORES->unk_9d; row++) {
        ScoreList_DrawRow(x, y + offset, row, tally);
        offset += SCORES->rows[row].height;
    }
}
