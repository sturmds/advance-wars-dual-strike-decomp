#include "types.h"
#include "game/task.h"

extern TaskCommandFunc gTaskCommands[];

/* Runs the task's script commands until a handler returns 0, the task
 * ends or gets an update function. Task_Create calls it once on a new
 * task; Task_RunTree calls it each frame for tasks without an update.
 * Needed to match: the first command is written out before the loop. */
void Task_RunScript(Task* task) {
  if (task->script == NULL || task->childCount != 0 || task->update != NULL) {
    return;
  }
  if (!gTaskCommands[task->pc->op](task)) {
    return;
  }
  do {
    if (task->script == NULL || task->childCount != 0 || task->update != NULL) {
      return;
    }
  } while (gTaskCommands[task->pc->op](task));
}
