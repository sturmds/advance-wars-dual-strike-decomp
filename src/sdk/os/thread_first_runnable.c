#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_022a9930;

OSThread *Thread_FirstRunnable(void)
{
    OSThread *thread = data_022a9930.list;

    while (thread != NULL && thread->state != OS_THREAD_STATE_READY) {
        thread = thread->next;
    }
    return thread;
}
