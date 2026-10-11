#include "types.h"
#include "sdk/os.h"

extern OSThread **data_022a9924;

void OS_RescheduleThread(void);

/* queue->head is a bit mask of the ids of the waiting threads. */
void OS_SleepThread(OSThreadQueue *queue)
{
    int irqState = OS_DisableInterrupts();
    OSThread *current = *data_022a9924;

    if (queue != NULL) {
        queue->head |= (u16)(1 << current->id);
    }
    current->state = OS_THREAD_STATE_WAITING;
    OS_RescheduleThread();
    OS_RestoreInterrupts(irqState);
}
