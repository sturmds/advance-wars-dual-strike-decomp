#include "types.h"
#include "game/task.h"

void Task_DestroyByScript(void* script);

/* Task script command: destroys the tasks running the script the command's
 * value points to, then moves on to the next command. */
BOOL func_020233c0(Task* task) {
  Task_DestroyByScript((void*)task->pc->value);
  task->pc++;
  return TRUE;
}
