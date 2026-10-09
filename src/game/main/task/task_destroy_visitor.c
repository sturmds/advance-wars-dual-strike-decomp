#include "types.h"
#include "game/task.h"

void func_02023958(Task *task);

/* Callback that Task_DestroyByScript hands to Task_ForEachByScript. */
void Task_DestroyVisitor(Task *task)
{
    func_02023958(task);
}
