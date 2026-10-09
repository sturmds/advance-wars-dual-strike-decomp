#include "types.h"
#include "game/task.h"

int func_0207bd80(void);
extern const TaskScriptCmd data_0216065c[];

void func_020811e4(Task *parent)
{
    if (func_0207bd80() == 1) {
        Task_CreateChild(data_0216065c, parent);
    }
}
