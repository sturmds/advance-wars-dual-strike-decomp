#include "types.h"
#include "game/task.h"

void func_02004dd4(int arg);

BOOL func_02022f4c(Task* task) {
  func_02004dd4(task->pc->arg);
  task->pc++;
  return TRUE;
}
