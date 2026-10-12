#include "types.h"
#include "game/task.h"

void Task_SetDestroyCallback(Task* task, void (*onDestroy)(Task* task));

/* Task script command: sets the task's destroy callback to the function the
 * command's value points to, then moves on to the next command. */
BOOL func_020234b0(Task* task) {
  Task_SetDestroyCallback(task, (void (*)(Task*))task->pc->value);
  task->pc++;
  return TRUE;
}
