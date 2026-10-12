#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_ov002_023604c4[];

void func_ov002_0235323c(void) {
  Task_Create(data_ov002_023604c4, TASK_LIST(1));
}
