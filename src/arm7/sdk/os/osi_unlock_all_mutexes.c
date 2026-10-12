#include "types.h"
#include "sdk/os_arm7.h"

void Thread_ReadyAllWaiters_Arm7(OSThreadQueue *queue);
OSMutex *func_arm7_037fc500(OSThread *thread);

/* ARM7 copy of Mutex_ReleaseAllHeld; main already has that symbol. Releases every mutex the thread holds and wakes the threads waiting on them. */
void func_arm7_037fc574(OSThread *thread)
{
    OSMutex *mutex;

    while (thread->mutexQueue.head != NULL) {
        mutex = func_arm7_037fc500(thread);
        mutex->count = 0;
        mutex->thread = NULL;
        Thread_ReadyAllWaiters_Arm7(&mutex->queue);
    }
}
