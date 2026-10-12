#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168bc4[];

void func_020e8860(void) { Task_Create(data_02168bc4, TASK_LIST(3)); }
