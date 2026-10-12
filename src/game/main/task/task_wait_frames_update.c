#include "types.h"
#include "game/task.h"

void Task_ResumeScript(Task* task);

/* Update callback set by TaskCmd_WaitFrames: counts the wait down each
 * frame and resumes the script when it reaches 0. */
void TaskWait_Update(Task* task) {
  task->unk_24--;
  if (task->unk_24 == 0) {
    Task_ResumeScript(task);
  }
}
