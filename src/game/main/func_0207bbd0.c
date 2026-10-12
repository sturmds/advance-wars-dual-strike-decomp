#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215fea0[];
void func_0207a3e0(void);

void func_0207bbd0(void) {
  Task_Create(data_0215fea0, TASK_LIST(0));
  func_0207a3e0();
}
