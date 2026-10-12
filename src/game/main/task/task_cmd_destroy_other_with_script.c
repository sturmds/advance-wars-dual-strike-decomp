#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;

void Task_Destroy(Task* task);

/* Script command: destroys the first other task that runs the same script,
 * so only one copy keeps running. */
BOOL TaskCmd_DestroyOtherWithScript(Task* task) {
  Task* other;
  int i;

  other = data_027c027c->taskPool;
  for (i = 0; i < TASK_COUNT; i++, other++) {
    if (other != task && other->script == task->script) {
      Task_Destroy(other);
      break;
    }
  }
  task->pc++;
  return TRUE;
}
