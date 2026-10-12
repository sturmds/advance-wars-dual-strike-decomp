#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02166d98[];

/* Starts the data_02166d98 task on top-level task list 3. */
void Map_StartBattleTask(void) { Task_Create(data_02166d98, TASK_LIST(3)); }
