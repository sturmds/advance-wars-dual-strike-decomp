#include "types.h"
#include "game/Unk_0229c734.h"

int ScoreList_HasMoreRows(void);

/* 40 when more rows follow this page (then the page maximum decides, else
 * the given points), 24 otherwise; plus 8 for 100 or more points and 4 for
 * 10 or more, which looks like room for the digits. */
int ScoreList_GetPanelSize(int points)
{
    int size = 40;

    if (ScoreList_HasMoreRows() == 1) {
        points = data_0229c734->pageMaxPoints;
    } else {
        size -= 16;
    }
    if (points >= 100) {
        size += 8;
    } else if (points >= 10) {
        size += 4;
    }
    return size;
}
