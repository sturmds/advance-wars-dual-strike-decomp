#include "types.h"
#include "game/task.h"

void func_02004230(int a0, Task* task);

BOOL func_02023004(Task* task) {
  func_02004230(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
