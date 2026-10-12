#include "types.h"
#include "sdk/os_arm7.h"

/* While nonzero, the rescheduler does not switch threads. */
extern u32 gSchedulerHoldDepth_Arm7;

/*
 * ARM7 copy of OS_EnableScheduler: undoes one Thread_HoldSwitching_Arm7.
 * Returns the count before the call.
 */
u32 Thread_ReleaseSwitching_Arm7(void)
{
    int irqState = Cpu_MaskIrq_Arm7();
    u32 count = 0;

    if (gSchedulerHoldDepth_Arm7 != 0) {
        count = gSchedulerHoldDepth_Arm7--;
    }
    Cpu_RestoreIrqBit_Arm7(irqState);
    return count;
}
