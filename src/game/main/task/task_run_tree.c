#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern u32 gTaskSystemReady;

void Task_RunScript(Task* task);

/* Runs one frame of task, of the tasks after it in its list (those first),
 * then of its children. A task waiting for children does not run, and
 * list 0 does not run while gTaskSystemReady is 0. */
void Task_RunTree(Task* task) {
  if (task == data_027c027c->taskLists[0] && !gTaskSystemReady) {
    return;
  }
  if (task->next != NULL) {
    Task_RunTree(task->next);
  }
  if (task->childCount == 0 && !(task->flags & TASK_FLAG_CREATING)) {
    if (task->update == NULL) {
      Task_RunScript(task);
    }
    if (task->update != NULL) {
      task->update(task);
    }
    if (task->flags & TASK_FLAG_FREED) {
      return;
    }
  }
  if (task->firstChild != NULL) {
    Task_RunTree(task->firstChild);
  }
}
