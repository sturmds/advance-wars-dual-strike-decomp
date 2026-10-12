#include "types.h"
#include "sdk/os_arm7.h"

extern vu64 gOsiTickCounter_Arm7;
extern u32 gOsiNeedResetTimer_Arm7;
void func_arm7_037fb6fc(int timer, void (*handler)(void *), void *arg);

/*
 * ARM7 copy of OSi_CountUpTick, the timer 0 overflow handler: counts one
 * more overflow and arms itself for the next.
 */
void Tick_OnTimer0Overflow_Arm7(void *arg)
{
    gOsiTickCounter_Arm7++;
    if (gOsiNeedResetTimer_Arm7 != 0) {
        REG_TM0CNT_H = 0;
        REG_TM0CNT_L = 0;
        REG_TM0CNT_H = TMCNT_START_IRQ_DIV64;
        gOsiNeedResetTimer_Arm7 = 0;
    }
    func_arm7_037fb6fc(0, Tick_OnTimer0Overflow_Arm7, NULL);
}
