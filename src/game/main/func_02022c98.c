#include "types.h"
#include "game/task.h"

void func_020063c0(int arg);

BOOL func_02022c98(Task* task) {
  func_020063c0(task->pc->arg);
  task->pc++;
  return TRUE;
}
