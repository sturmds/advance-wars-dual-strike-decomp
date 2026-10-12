#include "types.h"
#include "game/task.h"

void func_0200432c(int a0, Task* task);

BOOL func_02023034(Task* task) {
  func_0200432c(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
