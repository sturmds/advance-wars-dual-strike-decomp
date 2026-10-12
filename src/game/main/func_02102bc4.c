#include "types.h"
#include "game/task.h"

void func_0208be18(int arg);
extern const TaskScriptCmd data_0216d6d4[];

void func_02102bc4(void) {
  func_0208be18(1);
  Task_Create(data_0216d6d4, TASK_LIST(3));
}
