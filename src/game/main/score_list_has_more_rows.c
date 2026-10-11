#include "types.h"
#include "game/Unk_0229c734.h"

/* TRUE when the row right after the current page is in use (kind not 0). */
BOOL ScoreList_HasMoreRows(void)
{
    return data_0229c734->rows[data_0229c734->unk_9c + data_0229c734->unk_9d].kind != 0;
}
