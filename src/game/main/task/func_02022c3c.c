#include "types.h"
#include "game/task.h"

void func_0200640c(int a0, Task* task);

BOOL func_02022c3c(Task* task) {
  func_0200640c(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
