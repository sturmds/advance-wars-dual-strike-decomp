#include "types.h"
#include "game/task.h"

void func_02006398(int arg);

BOOL func_02022c6c(Task* task) {
  func_02006398(task->pc->arg);
  task->pc++;
  return TRUE;
}
