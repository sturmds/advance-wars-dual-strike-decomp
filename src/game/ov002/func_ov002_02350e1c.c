#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_ov002_02360044[];

void func_ov002_02350e1c(void) {
  Task_Create(data_ov002_02360044, TASK_LIST(1));
}
