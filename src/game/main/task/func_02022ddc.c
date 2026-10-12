#include "types.h"
#include "game/task.h"

void func_02004a1c(int a0, Task* task);

BOOL func_02022ddc(Task* task) {
  func_02004a1c(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
