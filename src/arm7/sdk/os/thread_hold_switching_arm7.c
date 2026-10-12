#include "types.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

extern u32 gSchedulerHoldDepth_Arm7;

/*
 * ARM7 copy of OS_DisableScheduler: stops thread switches until
 * Thread_ReleaseSwitching_Arm7 is called as many times. Returns the count before
 * the call; at the maximum count it returns garbage, as the original does.
 */
u32 Thread_HoldSwitching_Arm7(void)
{
    int irqState = func_arm7_037fdb80();
    u32 count;

    if (gSchedulerHoldDepth_Arm7 < 0xffffffff) {
        count = gSchedulerHoldDepth_Arm7++;
    }
    func_arm7_037fdb94(irqState);
    return count;
}
