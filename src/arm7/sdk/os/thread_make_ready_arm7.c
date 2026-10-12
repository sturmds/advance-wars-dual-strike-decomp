#include "types.h"
#include "sdk/os_arm7.h"

/* ARM7 copy of OS_WakeupThreadDirect: makes thread ready and reschedules. */
void Thread_MakeReady_Arm7(OSThread *thread)
{
    int irqState = Cpu_MaskIrq_Arm7();

    thread->state = OS_THREAD_STATE_READY;
    Thread_SwitchToFirstReady_Arm7();
    Cpu_RestoreIrqBit_Arm7(irqState);
}
