#include "types.h"
#include "game/task.h"

void func_02004b14(int a0, Task* task);

BOOL func_02022cf4(Task* task) {
  func_02004b14(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
