#include "types.h"
#include "game/task.h"

/* Starts script from its first command (unk_08 holds the current command)
 * and clears update so the script runs on the next frame. */
void Task_SetScript(Task *task, void *script)
{
    task->unk_04 = script;
    task->unk_08 = script;
    task->update = NULL;
}
