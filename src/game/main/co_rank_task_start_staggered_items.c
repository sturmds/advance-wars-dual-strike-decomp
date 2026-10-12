#include "types.h"
#include "game/co_rank_task.h"
#include "game/staggered_task.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215af94[];

/* Starts the task that handles one item per 16 frames (func_02048f80). Bit n
 * of its pending mask is set for each CO whose frame is nonzero; no task
 * when neither is. */
void CoRankTask_StartStaggeredItems(CoRankTask* task) {
  int mask = 0;
  int count = 0;
  StaggeredTask* staggered;

  if (task->cos[0].frame != 0) {
    mask |= 1;
    count++;
  }
  if (task->cos[1].frame != 0) {
    count++;
    mask |= 2;
  }
  if (count == 0) {
    return;
  }
  staggered = (StaggeredTask*)Task_Create(data_0215af94, TASK_LIST(3));
  staggered->pending = mask;
  staggered->itemsLeft = count;
}
