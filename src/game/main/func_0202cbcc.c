#include "types.h"
#include "game/task.h"

int func_0207bd80(void);
extern const TaskScriptCmd data_02159988[];

void func_0202cbcc(Task *parent)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(data_02159988, parent);
    }
}
