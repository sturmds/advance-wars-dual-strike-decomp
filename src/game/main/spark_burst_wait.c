#include "types.h"
#include "game/spark_burst_task.h"

void Task_ResumeScript(SparkBurstTask* task);

/* Script step: counts frames up to waitFrames, then resumes the script. */
void SparkBurst_Wait(SparkBurstTask* task) {
  if (task->frame != task->waitFrames) {
    task->frame++;
    return;
  }
  task->frame = 0;
  Task_ResumeScript(task);
}
