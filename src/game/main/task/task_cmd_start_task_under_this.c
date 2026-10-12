#include "types.h"
#include "game/task.h"

/* Script command: starts a task running the script in the command's value
 * with this task as its parent, and goes on without waiting for it. */
BOOL TaskCmd_StartTaskUnderThis(Task* task) {
  Task_Create((const TaskScriptCmd*)task->pc->value, task);
  task->pc++;
  return TRUE;
}
