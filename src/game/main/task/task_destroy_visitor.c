#include "types.h"
#include "game/task.h"

void func_02023958(Task *task);

/* Callback that Task_DestroyByScript hands to func_02023674. */
void Task_DestroyVisitor(Task *task)
{
    func_02023958(task);
}
