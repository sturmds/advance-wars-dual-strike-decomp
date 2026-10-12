#include "types.h"
#include "game/task.h"

/* Script command: calls the function in the command's value with the task.
 * While it returns 1, the task stays on this command and calls it again on
 * the next run. */
BOOL TaskCmd_CallWhileBusy(Task* task) {
  BOOL (*func)(Task* task) = (BOOL (*)(Task*))task->pc->value;

  task->pc++;
  if (func(task) == 1) {
    task->pc--;
    return FALSE;
  }
  return TRUE;
}
