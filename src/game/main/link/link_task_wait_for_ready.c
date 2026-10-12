#include "types.h"
#include "game/task.h"

int func_0207bd94(void);
void Link_FlagError(void);
void Task_ResumeScript(Task* task);

/* Task step. Resumes the script once func_0207bd94 returns 1 and flags a link
 * error after 300 frames without it. */
void LinkTask_WaitForReady(Task* task) {
  if (func_0207bd94() == 1) {
    task->waitFrames = 0;
    Task_ResumeScript(task);
  } else {
    task->waitFrames++;
    if (task->waitFrames == 300) {
      Link_FlagError();
    }
  }
}
