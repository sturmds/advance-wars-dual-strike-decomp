#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* While nonzero, the rescheduler does not switch threads. */
extern u32 gSchedulerHoldDepth_Arm7;

/*
 * ARM7 copy of OS_EnableScheduler: undoes one Thread_HoldSwitching_Arm7.
 * Returns the count before the call.
 */
u32 Thread_ReleaseSwitching_Arm7(void)
{
    int irqState = func_arm7_037fdb80();
    u32 count = 0;

    if (gSchedulerHoldDepth_Arm7 != 0) {
        count = gSchedulerHoldDepth_Arm7--;
    }
    func_arm7_037fdb94(irqState);
    return count;
}
