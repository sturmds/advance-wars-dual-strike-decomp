#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

void Task_Destroy(Task* task);

/* Script command: lets the task go on only if no other task runs the same
 * script; otherwise destroys the task. */
BOOL TaskCmd_ExitIfScriptRunning(Task* task) {
  Task* other;
  int i;
  int count;

  count = i = 0;
  other = data_027c027c->taskPool;
  for (; i < TASK_COUNT; i++, other++) {
    if (other->script == task->script) {
      count++;
    }
  }
  if (count <= 1) {
    task->pc++;
    return TRUE;
  }
  Task_Destroy(task);
  return FALSE;
}
