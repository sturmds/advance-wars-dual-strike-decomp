#include "types.h"
#include "game/task.h"

Task* Task_Alloc(void);
void Task_AddToList(Task* task, int level);
void Task_AddChild(Task* task, Task* parent);
void Task_RunScript(Task* task);

/* Creates a task that runs script and runs its first commands at once.
 * parentOrList below TASK_LEVELS: the top-level list to join. Otherwise
 * it is the parent task (see Task_CreateChild). If the script ends during
 * that first run, the task is already freed and its script is NULL. */
Task* Task_Create(const TaskScriptCmd* script, void* parentOrList) {
  Task* task = Task_Alloc();

  task->script = script;
  task->scriptStart = script;
  task->pc = script;
  task->onDestroy = NULL;
  task->update = NULL;
  task->parent = NULL;
  task->firstChild = NULL;
  task->prev = NULL;
  task->next = NULL;
  task->unk_24 = 0;
  task->group = 0;
  task->childCount = 0;
  task->flags = TASK_FLAG_CREATING;
  if ((int)parentOrList < TASK_LEVELS) {
    Task_AddToList(task, (int)parentOrList);
  } else {
    Task_AddChild(task, parentOrList);
  }
  Task_RunScript(task);
  task->flags &= ~TASK_FLAG_CREATING;
  return task;
}
