#include "types.h"
#include "game/task.h"
#include "game/spark_burst_task.h"

extern const TaskScriptCmd data_0215c2fc[];

/* Starts a burst centred on (x, y) after waitFrames frames. Stores
 * waitFrames - 1 so that 0 gives a negative count, which SparkBurst_Init
 * takes as no wait. */
void SparkBurst_Start(int x, int y, int waitFrames) {
  SparkBurstTask* task =
      (SparkBurstTask*)Task_Create(data_0215c2fc, TASK_LIST(3));

  task->x = x;
  task->y = y;
  task->waitFrames = waitFrames - 1;
}
