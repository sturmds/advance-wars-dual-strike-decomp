/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "types.h"
#include "sdk/card.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void Thread_Reprioritize_Arm7(void *thread, u32 priority);

/* Sets the card thread's priority and returns the old one. */
u32 CARD_SetThreadPriority(u32 priority)
{
    CARDiArm7Common *common = &data_arm7_03809280;
    int interrupts = func_arm7_037fdb80();
    u32 old = common->priority;

    common->priority = priority;
    Thread_Reprioritize_Arm7(common->thread, common->priority);
    func_arm7_037fdb94(interrupts);
    return old;
}
