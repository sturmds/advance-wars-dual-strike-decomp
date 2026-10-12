#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

/* ARM7 copy of OS_UnlockMutex; main already has that symbol. Drops one level of nesting; the last level frees the mutex and wakes its waiters. */
void Mutex_Release_Arm7(OSMutex *mutex)
{
    int irqState = Cpu_MaskIrq_Arm7();
    OSThread *current = data_arm7_0380790c.current;

    if (mutex->thread == current) {
        mutex->count--;
        if (mutex->count == 0) {
            Mutex_UnlinkFromOwner_Arm7(current, mutex);
            mutex->thread = NULL;
            Thread_ReadyAllWaiters_Arm7(&mutex->queue);
        }
    }
    Cpu_RestoreIrqBit_Arm7(irqState);
}
