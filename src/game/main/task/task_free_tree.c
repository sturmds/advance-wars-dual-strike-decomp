#include "types.h"
#include "game/task.h"

void Task_Free(Task* task);

/* Frees task, the tasks after it in its list and all their children.
 * Each task's destroy callback (onDestroy, see Task_SetDestroyCallback) runs
 * before it goes back on the free stack. */
void Task_FreeTree(Task* task) {
  void (*onDestroy)(Task* task);

  if (task->next != NULL) {
    Task_FreeTree(task->next);
  }
  if (task->firstChild != NULL) {
    Task_FreeTree(task->firstChild);
  }
  if (task->flags & TASK_FLAG_FREED) {
    return;
  }
  onDestroy = task->onDestroy;
  if (onDestroy != NULL) {
    onDestroy(task);
  }
  Task_Free(task);
  task->script = NULL;
  task->scriptStart = NULL;
  task->update = NULL;
  task->flags |= TASK_FLAG_FREED;
  if (task->flags & TASK_FLAG_CHILD) {
    task->parent->childCount--;
  }
}
