#include "types.h"
#include "game/task.h"

void func_02004ae4(int a0, Task* task);

BOOL func_02022cc4(Task* task) {
  func_02004ae4(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
