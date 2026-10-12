#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0216d8ac[];

/* Starts the data_0216d8ac task on top-level task list 3. */
void func_0210375c(void) { Task_Create(data_0216d8ac, TASK_LIST(3)); }
