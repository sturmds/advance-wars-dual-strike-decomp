#include "types.h"
#include "game/task.h"

void func_02004dfc(int arg);

BOOL func_02022f78(Task* task) {
  func_02004dfc(task->pc->arg);
  task->pc++;
  return TRUE;
}
