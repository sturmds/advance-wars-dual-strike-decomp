#include "types.h"
#include "game/task.h"

int func_0207bd80(void);
extern const TaskScriptCmd data_02160534[];

void func_02080684(Task *parent)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(data_02160534, parent);
    }
}
