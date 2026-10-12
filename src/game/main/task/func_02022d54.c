#include "types.h"
#include "game/task.h"

void func_02004db0(int a0, Task* task);

BOOL func_02022d54(Task* task) {
  func_02004db0(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
