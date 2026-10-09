#include "types.h"
#include "game/task.h"

/* unk_0c runs when the task is destroyed (see func_0202397c). */
void Task_SetDestroyCallback(Task *task, void (*onDestroy)(Task *task))
{
    task->unk_0c = onDestroy;
}
