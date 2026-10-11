#include "types.h"
#include "game/Unk_0229c734.h"

/* Counts the rows on the current page of the score list (unk_9c is the first
 * row, unk_9d the number of rows) that give at least this many points. */
int ScoreList_CountRowsAtLeast(int points)
{
    int i;
    int count = 0;

    for (i = data_0229c734->unk_9c; i < data_0229c734->unk_9c + data_0229c734->unk_9d; i++) {
        if (points <= data_0229c734->rows[i].points) {
            count++;
        }
    }
    return count;
}
