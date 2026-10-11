#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_0215ffdc[];
extern const TaskScriptCmd data_02160044[];

Task *Task_FindByScript(const TaskScriptCmd *script);

/* Returns the task running the data_0215ffdc script, else the one running data_02160044. */
Task *func_0207ca34(void)
{
    Task *task = Task_FindByScript(data_0215ffdc);

    if (task != NULL) {
        return task;
    }
    return Task_FindByScript(data_02160044);
}
