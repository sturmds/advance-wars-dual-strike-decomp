#include "types.h"
#include "game/spark_burst_task.h"

u32 Rng_Next(void);
void Task_JumpToLabel(SparkBurstTask* task, int label);

/* Script step: picks a random style (0-15) for each spark. With no wait
 * requested it jumps to label 0 of the script, past the wait step. */
void SparkBurst_Init(SparkBurstTask* task) {
  int i;

  for (i = 0; i < SPARK_BURST_SPARKS; i++) {
    task->sparkStyles[i] = Rng_Next() & 15;
  }
  task->frame = 0;
  if (task->waitFrames < 0) {
    Task_JumpToLabel(task, 0);
  }
}
