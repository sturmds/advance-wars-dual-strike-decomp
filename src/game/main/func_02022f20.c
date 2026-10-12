#include "types.h"
#include "game/task.h"

void func_02004d64(int arg);

BOOL func_02022f20(Task* task) {
  func_02004d64(task->pc->arg);
  task->pc++;
  return TRUE;
}
