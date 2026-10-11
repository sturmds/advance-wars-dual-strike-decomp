#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_02167fb8[];

BOOL func_020d9934(void);

/* Starts the data_02167fb8 task when func_020d9934 says the map has an object of kind 9 or 10. */
void ObjectHealScan_Start(void)
{
    if (func_020d9934()) {
        Task_Create(data_02167fb8, TASK_LIST(3));
    }
}
