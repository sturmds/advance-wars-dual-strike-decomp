#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02159e20[];

void func_02031f54(void) { Task_Create(data_02159e20, TASK_LIST(3)); }
