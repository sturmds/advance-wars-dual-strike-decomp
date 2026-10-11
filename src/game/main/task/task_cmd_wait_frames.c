#include "types.h"
#include "game/task.h"

void func_02023234(Task *task);

/* Script command: waits for as many frames as the command's arg says,
 * then goes on with the next command. An arg of 0 does not wait. */
BOOL TaskCmd_WaitFrames(Task *task)
{
    s16 frames = task->pc->arg;

    if (frames != 0) {
        task->unk_24 = frames;
        task->update = func_02023234;
    }
    task->pc++;
    return FALSE;
}
