#include "types.h"
#include "game/task.h"

int func_0207bd80(void);
extern const TaskScriptCmd data_021673a4[];

void func_020d6a34(Task *parent)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(data_021673a4, parent);
    }
}
