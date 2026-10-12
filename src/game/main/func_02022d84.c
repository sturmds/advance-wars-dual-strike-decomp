#include "types.h"
#include "game/task.h"

void func_02004b44(int arg);

BOOL func_02022d84(Task* task) {
  func_02004b44(task->pc->arg);
  task->pc++;
  return TRUE;
}
