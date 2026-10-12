#include "types.h"
#include "game/task.h"

void func_020042d8(int a0, Task* task);

BOOL func_02022fa4(Task* task) {
  func_020042d8(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
