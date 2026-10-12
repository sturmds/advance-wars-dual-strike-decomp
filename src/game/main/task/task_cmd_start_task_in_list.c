#include "types.h"
#include "game/task.h"

/* Script command: starts a task running the script in the command's value,
 * in the top-level list that arg names. */
BOOL TaskCmd_StartTaskInList(Task* task) {
  Task_Create((const TaskScriptCmd*)task->pc->value, TASK_LIST(task->pc->arg));
  task->pc++;
  return TRUE;
}
