#include "types.h"
#include "game/task.h"

extern TaskScriptCmd data_02157324[];
Task *Task_Create(const TaskScriptCmd *script, void *parentOrList);
void func_02004bac(Task *task);

void func_02004ab0(u16 arg)
{
    Task *task = Task_Create(data_02157324, (void *)3);

    if (task != NULL) {
        task->unk_64 = arg;
        func_02004bac(task);
    }
}
