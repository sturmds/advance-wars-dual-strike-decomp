#include "types.h"
#include "game/task.h"

/* Script command: calls the function in the command's value with the task,
 * ignores its result, and goes on with the next command. */
BOOL TaskCmd_CallAndContinue(Task* task) {
  void (*func)(Task* task) = (void (*)(Task*))task->pc->value;

  task->pc++;
  func(task);
  return TRUE;
}
