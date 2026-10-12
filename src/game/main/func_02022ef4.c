#include "types.h"
#include "game/task.h"

void func_02004d3c(int arg);

BOOL func_02022ef4(Task* task) {
  func_02004d3c(task->pc->arg);
  task->pc++;
  return TRUE;
}
