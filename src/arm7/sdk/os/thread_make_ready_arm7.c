#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void Thread_SwitchToFirstReady_Arm7(void);

/* ARM7 copy of OS_WakeupThreadDirect: makes thread ready and reschedules. */
void Thread_MakeReady_Arm7(OSThread *thread)
{
    int irqState = func_arm7_037fdb80();

    thread->state = OS_THREAD_STATE_READY;
    Thread_SwitchToFirstReady_Arm7();
    func_arm7_037fdb94(irqState);
}
