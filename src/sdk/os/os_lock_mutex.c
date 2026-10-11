#include "types.h"
#include "sdk/os.h"

extern OSThreadInfo OSi_ThreadInfo;

void Mutex_AppendToOwner(OSThread *thread, OSMutex *mutex);
void OS_SleepThread(OSThreadQueue *queue);

/* Takes the mutex, sleeping until its owner releases it. The owner may lock again. */
void OS_LockMutex(OSMutex *mutex)
{
    int irqState = OS_DisableInterrupts();
    OSThread *current = OSi_ThreadInfo.current;

    while (TRUE) {
        if (mutex->thread == NULL) {
            mutex->thread = current;
            mutex->count++;
            Mutex_AppendToOwner(current, mutex);
            break;
        }
        if (mutex->thread == current) {
            mutex->count++;
            break;
        }
        current->mutex = mutex;
        OS_SleepThread(&mutex->queue);
        current->mutex = NULL;
    }
    OS_RestoreInterrupts(irqState);
}
