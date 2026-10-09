#include "types.h"
#include "game/task.h"

int func_0207bd80(void);
extern const TaskScriptCmd data_02160624[];

void func_020811b4(Task *parent)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(data_02160624, parent);
    }
}
