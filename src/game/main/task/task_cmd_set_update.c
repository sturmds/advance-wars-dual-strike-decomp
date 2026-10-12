#include "types.h"
#include "game/task.h"

/* Script command: makes the function in the command's value the task's
 * update callback, and ends the task's turn. */
BOOL TaskCmd_SetUpdate(Task* task) {
  task->update = (void (*)(Task*))task->pc->value;
  task->pc++;
  return FALSE;
}
