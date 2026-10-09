#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"

extern Unk_02080440 *data_027c027c;

void Task_Destroy(Task *task);

/* Destroys every task in the pool whose unk_26 equals group. */
void Task_DestroyGroup(int group)
{
    Task *task;
    int i;

    task = data_027c027c->taskPool;
    for (i = 0; i < TASK_COUNT; i++, task++) {
        if (task->unk_26 == group) {
            Task_Destroy(task);
        }
    }
}
