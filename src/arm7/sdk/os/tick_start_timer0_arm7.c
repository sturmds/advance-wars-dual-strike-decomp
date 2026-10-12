#include "types.h"
#include "sdk/os_arm7.h"

extern u16 data_arm7_03807a7c;
extern vu64 gOsiTickCounter_Arm7;
extern u32 gOsiNeedResetTimer_Arm7;

/*
 * ARM7 copy of OS_InitTick. Once only: zeroes the tick counter and starts
 * timer 0, counting up with an interrupt on overflow.
 */
void Tick_StartTimer0_Arm7(void)
{
    if (data_arm7_03807a7c != 0) {
        return;
    }
    data_arm7_03807a7c = 1;
    Timer_MarkInUse_Arm7(0);
    gOsiTickCounter_Arm7 = 0;
    REG_TM0CNT_H = 0;
    REG_TM0CNT_L = 0;
    REG_TM0CNT_H = TMCNT_START_IRQ_DIV64;
    /* the handler takes an argument because it also serves as a timer slot handler */
    Irq_InstallHandlers_Arm7(IRQ_MASK_TIMER0, (OSIrqFunction)Tick_OnTimer0Overflow_Arm7);
    Irq_SetEnableBits_Arm7(IRQ_MASK_TIMER0);
    gOsiNeedResetTimer_Arm7 = 0;
}
