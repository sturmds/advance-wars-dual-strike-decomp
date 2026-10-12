#include "types.h"
#include "game/task.h"

void func_02004d8c(int a0, Task* task);

BOOL func_02022d24(Task* task) {
  func_02004d8c(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
