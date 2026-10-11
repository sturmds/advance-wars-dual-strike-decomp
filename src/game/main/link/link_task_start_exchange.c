#include "types.h"
#include "game/link_wait_task.h"

int func_0207bd80(void);
int func_0207a1ac(void);
void func_0207b564(void);

/* In link mode, when func_0207a1ac returns 0, runs func_0207b564 (link state 0
 * to 1) and restarts the wait counter that LinkTask_WaitForPeer uses. */
void LinkTask_StartExchange(LinkWaitTask *task)
{
    if (func_0207bd80() != 1) {
        return;
    }
    if (func_0207a1ac() != 0) {
        return;
    }
    func_0207b564();
    task->waitFrames = 0;
}
