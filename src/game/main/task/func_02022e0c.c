#include "types.h"
#include "game/task.h"

void func_02004a4c(int a0, Task* task);

BOOL func_02022e0c(Task* task) {
  func_02004a4c(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
