#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0216d644[];

/* Starts the data_0216d644 task on top-level task list 3. */
void func_02102c34(void) { Task_Create(data_0216d644, TASK_LIST(3)); }
