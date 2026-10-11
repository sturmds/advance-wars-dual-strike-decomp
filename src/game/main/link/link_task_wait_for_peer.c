#include "types.h"
#include "game/link_wait_task.h"

int Link_IsActive(void);
int func_0207bd94(void);
void Link_FlagError(void);
void Task_ResumeScript(LinkWaitTask *task);

/* Task step. Outside link mode it resumes the script at once. In link mode it
 * resumes when the last peer check answered (func_0207bd94 returns 1) and
 * flags a link error after 300 frames without an answer. */
void LinkTask_WaitForPeer(LinkWaitTask *task)
{
    if (Link_IsActive() == 1) {
        if (func_0207bd94() == 1) {
            task->waitFrames = 0;
            Task_ResumeScript(task);
        } else {
            task->waitFrames++;
            if (task->waitFrames == 300) {
                Link_FlagError();
            }
        }
    } else {
        Task_ResumeScript(task);
    }
}
