#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

int ScoreList_GetPanelSize(int points);

/* Task step: resets the counters of the result point task and adds up the
 * points of the rows in use (a row of kind 0 ends the list). Stores the sum
 * and what ScoreList_GetPanelSize makes of it in the task. */
void ResultPoint_SumPoints(ResultPointTask *task)
{
    int i;

    task->frame = 0;
    task->tallyPopFrame = 0;
    task->countStep = 0;
    data_0229c734->tally = 0;
    task->totalPoints = 0;
    for (i = 0; i < SCORE_ROW_COUNT; i++) {
        if (data_0229c734->rows[i].kind == 0) {
            break;
        }
        task->totalPoints += data_0229c734->rows[i].points;
    }
    task->rank = ScoreList_GetPanelSize(task->totalPoints);
}
