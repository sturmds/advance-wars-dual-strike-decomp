#include "types.h"
#include "game/Unk_0229c734.h"

/* Stores the largest points value on the current page of the score list in
 * unk_98 (rows unk_9c up to unk_9c + unk_9d). */
void ScoreList_UpdatePageMax(void)
{
    int i;

    data_0229c734->unk_98 = 0;
    for (i = data_0229c734->unk_9c; i < data_0229c734->unk_9c + data_0229c734->unk_9d; i++) {
        if (data_0229c734->unk_98 < data_0229c734->rows[i].points) {
            data_0229c734->unk_98 = data_0229c734->rows[i].points;
        }
    }
}
