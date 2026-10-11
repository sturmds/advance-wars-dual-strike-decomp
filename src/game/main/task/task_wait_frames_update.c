#include "types.h"
#include "game/task.h"

void Task_ResumeScript(Task *task);

/* Update callback set by TaskCmd_WaitFrames: counts the wait down each
 * frame and resumes the script when it reaches 0. */
void TaskWait_Update(Task *task)
{
    s16 *frames = (s16 *)&task->unk_24; /* needed to match: signed halfword access */

    (*frames)--;
    if (*frames == 0) {
        Task_ResumeScript(task);
    }
}
