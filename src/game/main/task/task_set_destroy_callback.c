#include "types.h"
#include "game/task.h"

/* onDestroy runs when the task is destroyed (see Task_FreeTree). */
void Task_SetDestroyCallback(Task* task, void (*onDestroy)(Task* task)) {
  task->onDestroy = onDestroy;
}
