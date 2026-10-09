#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215d3d0[];

void func_0206d5b4(void)
{
    Task_Create(data_0215d3d0, TASK_LIST(3));
}
