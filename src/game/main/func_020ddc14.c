#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02166678[];

void func_020ddc14(void) { Task_Create(data_02166678, TASK_LIST(3)); }
