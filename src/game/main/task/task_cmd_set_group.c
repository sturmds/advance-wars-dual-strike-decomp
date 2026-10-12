#include "types.h"
#include "game/task.h"

/* Script command: puts the task in the group named by the command's arg
 * (see Task_DestroyGroup). */
BOOL TaskCmd_SetGroup(Task* task) {
  task->group = task->pc->arg;
  task->pc++;
  return TRUE;
}
