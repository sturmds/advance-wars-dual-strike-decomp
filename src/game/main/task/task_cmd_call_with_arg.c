#include "types.h"
#include "game/task.h"

/* Script command: calls the function in the command's value with arg and the
 * task, and returns its result. */
BOOL TaskCmd_CallWithArg(Task* task) {
  BOOL (*func)(int arg, Task* task) = (BOOL (*)(int, Task*))task->pc->value;
  int arg = task->pc->arg;

  task->pc++;
  return func(arg, task);
}
