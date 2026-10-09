#include "types.h"
#include "game/task.h"

int func_0207bd80(void);
extern const TaskScriptCmd data_021606ec[];

void func_02082038(Task *parent)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(data_021606ec, parent);
    }
}
