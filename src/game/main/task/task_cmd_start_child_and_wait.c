#include "types.h"
#include "game/task.h"

/* Script command: starts a child task running the script in the command's
 * value. The task's script holds until the child ends (Task.childCount). */
BOOL TaskCmd_StartChildAndWait(Task* task) {
  Task_CreateChild((const TaskScriptCmd*)task->pc->value, task);
  task->pc++;
  return FALSE;
}
