#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

/* Calls visit on every task in the pool that was started with script. */
void Task_ForEachByScript(void* script, void (*visit)(Task* task)) {
  Task* task;
  int i;

  task = data_027c027c->taskPool;
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->script == script) {
      visit(task);
    }
  }
}
