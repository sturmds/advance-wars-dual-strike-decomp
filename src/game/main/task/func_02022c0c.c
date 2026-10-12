#include "types.h"
#include "game/task.h"

void func_020063e8(int a0, Task* task);

BOOL func_02022c0c(Task* task) {
  func_020063e8(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
