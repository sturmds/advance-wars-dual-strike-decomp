#include "types.h"
#include "game/task.h"

void func_02004cf4(int a0, Task* task);

BOOL func_02022e94(Task* task) {
  func_02004cf4(task->pc->arg, task);
  task->pc++;
  return FALSE;
}
