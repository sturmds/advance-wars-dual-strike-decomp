#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0216a204[];

/* Starts the value ease task (EaseTask_Update) from `from` to `to` over
 * `frames` frames, under parent, or as a level 3 task when parent is NULL. */
Task* EaseTask_Start(int from, int to, int frames, Task* parent) {
  Task* task;

  if (parent != NULL) {
    task = Task_CreateChild(data_0216a204, parent);
  } else {
    task = Task_Create(data_0216a204, TASK_LIST(3));
  }
  task->unk_64 = from;
  task->unk_66 = to;
  task->unk_54 = frames;
  return task;
}
