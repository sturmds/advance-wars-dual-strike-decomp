#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern u32 gTaskSystemReady;

/* Clears every task, puts all of them on the free stack and empties the
 * top-level task lists. */
void Task_InitSystem(void) {
  int i;
  Task* task;

  for (i = 0; i < TASK_COUNT; i++) {
    task = &data_027c027c->taskPool[i];
    task->script = NULL;
    task->scriptStart = NULL;
    task->pc = NULL;
    task->onDestroy = NULL;
    task->update = NULL;
    task->parent = NULL;
    task->firstChild = NULL;
    task->prev = NULL;
    task->next = NULL;
    task->unk_24 = 0;
    task->group = 0;
    task->flags = 0;
    task->childCount = 0;
    data_027c027c->taskFreeStack[i] = task;
  }
  data_027c027c->taskFreeStack[TASK_COUNT] = NULL;
  data_027c027c->taskFreeTop = data_027c027c->taskFreeStack;

  for (i = 0; i < TASK_LEVELS; i++) {
    data_027c027c->taskLists[i] = NULL;
  }
  gTaskSystemReady = 1;
}
