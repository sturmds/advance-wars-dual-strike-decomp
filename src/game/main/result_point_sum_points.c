#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

u8 func_02039bf8(int points);

/* Task step: resets the counters of the result point task and adds up the
 * points of the rows in use (a row of kind 0 ends the list). Stores the sum
 * and what func_02039bf8 makes of it in the task. */
void ResultPoint_SumPoints(ResultPointTask *task)
{
    int i;

    task->frame = 0;
    task->unk_34 = 0;
    task->unk_38 = 0;
    data_0229c734->tally = 0;
    task->totalPoints = 0;
    for (i = 0; i < SCORE_ROW_COUNT; i++) {
        if (data_0229c734->rows[i].kind == 0) {
            break;
        }
        task->totalPoints += data_0229c734->rows[i].points;
    }
    task->rank = func_02039bf8(task->totalPoints);
}
