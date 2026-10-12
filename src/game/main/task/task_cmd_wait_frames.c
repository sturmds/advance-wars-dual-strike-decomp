#include "types.h"
#include "game/task.h"

void TaskWait_Update(Task* task);

/* Script command: waits for as many frames as the command's arg says,
 * then goes on with the next command. An arg of 0 does not wait. */
BOOL TaskCmd_WaitFrames(Task* task) {
  s16 frames = task->pc->arg;

  if (frames != 0) {
    task->unk_24 = frames;
    task->update = TaskWait_Update;
  }
  task->pc++;
  return FALSE;
}
