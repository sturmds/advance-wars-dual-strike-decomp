#include "types.h"
#include "game/task.h"

void Task_Unlink(Task* task);
void Task_FreeTree(Task* task);

/* Unlinks task and frees it with all its children. NULL is allowed. */
void Task_Destroy(Task* task) {
  if (task == NULL) {
    return;
  }
  Task_Unlink(task);
  Task_FreeTree(task);
}
