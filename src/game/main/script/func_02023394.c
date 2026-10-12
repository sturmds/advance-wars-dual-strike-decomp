#include "types.h"
#include "game/task.h"

void Task_ResumeByScript(void* script);

/* Task script command: resumes the tasks running the script the command's
 * value points to, then moves on to the next command. */
BOOL func_02023394(Task* task) {
  Task_ResumeByScript((void*)task->pc->value);
  task->pc++;
  return TRUE;
}
