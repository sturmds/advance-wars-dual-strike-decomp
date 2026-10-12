#include "types.h"
#include "game/task.h"

void func_02004d18(int a0, Task* task);

BOOL func_02022ec4(Task* task) {
  func_02004d18(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
