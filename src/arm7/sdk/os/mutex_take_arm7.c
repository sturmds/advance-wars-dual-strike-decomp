#include "types.h"
#include "sdk/os_arm7.h"

extern OSThreadInfo data_arm7_0380790c;

/* ARM7 copy of OS_LockMutex; main already has that symbol. Takes the mutex, sleeping until its owner releases it. The owner may lock again. */
void Mutex_Take_Arm7(OSMutex *mutex)
{
    int irqState = Cpu_MaskIrq_Arm7();
    OSThread *current = data_arm7_0380790c.current;

    while (TRUE) {
        if (mutex->thread == NULL) {
            mutex->thread = current;
            mutex->count++;
            Mutex_AppendToOwner_Arm7(current, mutex);
            break;
        }
        if (mutex->thread == current) {
            mutex->count++;
            break;
        }
        current->mutex = mutex;
        Thread_WaitOnQueue_Arm7(&mutex->queue);
        current->mutex = NULL;
    }
    Cpu_RestoreIrqBit_Arm7(irqState);
}
