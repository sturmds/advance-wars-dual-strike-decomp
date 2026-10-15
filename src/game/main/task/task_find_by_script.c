#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

/* Returns the first task in the pool started with script, or NULL. */
Task* Task_FindByScript(void* script) {
  Task* task;
  int i;

  task = data_027c027c->taskPool;
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->script == script) {
      return task;
    }
  }
  return NULL;
}
