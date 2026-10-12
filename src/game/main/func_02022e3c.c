#include "types.h"
#include "game/task.h"

void func_02004a7c(int arg);

BOOL func_02022e3c(Task* task) {
  func_02004a7c(task->pc->arg);
  task->pc++;
  return TRUE;
}
