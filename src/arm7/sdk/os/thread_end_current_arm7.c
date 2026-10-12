#include "types.h"
#include "sdk/os_arm7.h"

extern OSThread **gRunningThreadRef_Arm7;
extern OSThread *data_arm7_03807920[];   /* OSThreadInfo.threads, under its own symbol */

int func_arm7_037fdb80(void);
void func_arm7_037fdc10(void);
void func_arm7_037fc574(OSThread *thread);
void func_arm7_037fc18c(OSThread *thread);
void Thread_ReadyAllWaiters_Arm7(OSThreadQueue *queue);
void Thread_SwitchToFirstReady_Arm7(void);

/*
 * ARM7 copy of OS_ExitThread: ends the current thread. It frees its
 * mutexes and id, wakes the threads joined to it and never returns.
 */
void Thread_EndCurrent_Arm7(void)
{
    OSThread *current;

    func_arm7_037fdb80();
    current = *gRunningThreadRef_Arm7;
    func_arm7_037fc574(current);
    func_arm7_037fc18c(current);
    data_arm7_03807920[current->id] = NULL;
    current->state = OS_THREAD_STATE_TERMINATED;
    Thread_ReadyAllWaiters_Arm7(&current->joinQueue);
    Thread_SwitchToFirstReady_Arm7();
    func_arm7_037fdc10();
}
