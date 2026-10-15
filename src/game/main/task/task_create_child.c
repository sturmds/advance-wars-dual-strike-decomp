#include "types.h"
#include "game/task.h"

/* Creates a task under parent. The parent counts it in childCount and its
 * script waits while that is nonzero. Returns NULL when the child's script
 * already ended during its first run (the task is freed by then). */
Task* Task_CreateChild(const TaskScriptCmd* script, Task* parent) {
  Task* task = Task_Create(script, parent);

  if (task->script == NULL) {
    return NULL;
  }
  task->flags |= TASK_FLAG_CHILD;
  task->parent->childCount++;
  return task;
}
