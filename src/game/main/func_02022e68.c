#include "types.h"
#include "game/task.h"

void func_02004ab0(int arg);

BOOL func_02022e68(Task* task) {
  func_02004ab0(task->pc->arg);
  task->pc++;
  return TRUE;
}
