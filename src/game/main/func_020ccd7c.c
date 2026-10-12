#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02166568[];

void func_020ccd7c(void) { Task_Create(data_02166568, TASK_LIST(3)); }
