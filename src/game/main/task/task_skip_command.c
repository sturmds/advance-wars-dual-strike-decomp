#include "types.h"
#include "game/task.h"

/* Script command that does nothing: moves to the next command. */
BOOL Task_SkipCommand(Task *task)
{
    task->unk_08 = (TaskScriptCmd *)task->unk_08 + 1;
    return TRUE;
}
