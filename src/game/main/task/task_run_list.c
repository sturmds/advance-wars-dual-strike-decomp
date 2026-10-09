#include "types.h"
#include "game/task.h"

void func_02023794(Task *task);

/* Runs one frame of every task in a list (head: a taskLists entry) and
 * of their children. */
void Task_RunList(Task *task)
{
    if (task != NULL) {
        func_02023794(task);
    }
}
