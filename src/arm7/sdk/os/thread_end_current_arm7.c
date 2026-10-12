#include "types.h"
#include "sdk/os_arm7.h"

extern OSThread **gRunningThreadRef_Arm7;
extern OSThread *data_arm7_03807920[];   /* OSThreadInfo.threads, under its own symbol */

/*
 * ARM7 copy of OS_ExitThread: ends the current thread. It frees its
 * mutexes and id, wakes the threads joined to it and never returns.
 */
void Thread_EndCurrent_Arm7(void)
{
    OSThread *current;

    Cpu_MaskIrq_Arm7();
    current = *gRunningThreadRef_Arm7;
    Mutex_ReleaseAllHeld_Arm7(current);
    Thread_Unlink_Arm7(current);
    data_arm7_03807920[current->id] = NULL;
    current->state = OS_THREAD_STATE_TERMINATED;
    Thread_ReadyAllWaiters_Arm7(&current->joinQueue);
    Thread_SwitchToFirstReady_Arm7();
    Cpu_HaltForever_Arm7();
}
