#include "types.h"
#include "game/task.h"

void func_02004284(int a0, Task* task);

BOOL func_02022fd4(Task* task) {
  func_02004284(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
