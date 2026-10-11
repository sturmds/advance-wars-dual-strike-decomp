#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo data_022a9930;

void OS_RescheduleThread(void);

/* queue->head is a bit mask of the ids of the waiting threads. */
void OS_WakeupThread(OSThreadQueue *queue)
{
    s32 waiting;
    int irqState;
    OSThread *thread;

    irqState = OS_DisableInterrupts();
    waiting = queue->head;
    if (waiting != 0) {
        for (thread = data_022a9930.list; thread != NULL; thread = thread->next) {
            if (waiting & (1 << thread->id)) {
                thread->state = OS_THREAD_STATE_READY;
            }
        }
        queue->head = 0;
        OS_RescheduleThread();
    }
    OS_RestoreInterrupts(irqState);
}
