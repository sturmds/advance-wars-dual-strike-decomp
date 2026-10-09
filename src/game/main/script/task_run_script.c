#include "types.h"
#include "game/task.h"

extern TaskCommandFunc gTaskCommands[];

/* Runs the task's script commands until a handler returns 0, the task
 * ends or gets an update function. Task_Create calls it once on a new
 * task; Task_RunTree calls it each frame for tasks without an update.
 * Needed to match: the first command is written out before the loop. */
void Task_RunScript(Task *task)
{
    if (task->unk_00 == NULL || task->childCount != 0 || task->update != NULL) {
        return;
    }
    if (!gTaskCommands[((TaskScriptCmd *)task->unk_08)->op](task)) {
        return;
    }
    do {
        if (task->unk_00 == NULL || task->childCount != 0 || task->update != NULL) {
            return;
        }
    } while (gTaskCommands[((TaskScriptCmd *)task->unk_08)->op](task));
}
