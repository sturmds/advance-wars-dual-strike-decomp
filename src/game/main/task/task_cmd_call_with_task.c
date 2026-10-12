#include "types.h"
#include "game/task.h"

/* Script command: calls the function in the command's value with the task
 * and returns its result. */
BOOL TaskCmd_CallWithTask(Task* task) {
  BOOL (*func)(Task* task) = (BOOL (*)(Task*))task->pc->value;

  task->pc++;
  return func(task);
}
