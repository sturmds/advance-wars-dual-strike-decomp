#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

/* ARM7 copy of Thread_FirstRunnable: the first ready thread in priority order. */
OSThread *Thread_FirstRunnable_Arm7(void)
{
    OSThread *thread = data_arm7_0380790c.list;

    while (thread != NULL && thread->state != OS_THREAD_STATE_READY) {
        thread = thread->next;
    }
    return thread;
}
