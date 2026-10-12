#include "types.h"
#include "game/task.h"

void func_02004b78(int arg);

BOOL func_02022db0(Task* task) {
  func_02004b78(task->pc->arg);
  task->pc++;
  return TRUE;
}
