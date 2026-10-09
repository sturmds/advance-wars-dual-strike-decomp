#include "types.h"
#include "game/task.h"

extern TaskScriptCmd data_02157344[];
Task *Task_Create(const TaskScriptCmd *script, void *parentOrList);
void func_02004bac(Task *task);

void func_02004b44(u16 arg)
{
    Task *task = Task_Create(data_02157344, (void *)3);

    if (task != NULL) {
        task->unk_64 = arg;
        func_02004bac(task);
    }
}
