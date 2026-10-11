#include "types.h"
#include "game/task.h"
#include "game/name_entry_work.h"

extern const TaskScriptCmd data_0216ac14[];

/* Starts the name entry screen under parent. maxLength is 8 or 16.
 * Both arguments are read from the stack: the original is variadic. */
void func_020ffb04(Task *parent, ...)
{
    NameEntryTask *task;
    int *args = (int *)&parent;

    task = (NameEntryTask *)Task_CreateChild(data_0216ac14, (Task *)args[0]);
    task->maxLength = args[1];
}
