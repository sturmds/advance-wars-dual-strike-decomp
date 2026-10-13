#include "types.h"
#include "sdk/os_arm7.h"

/* bit n set: hardware timer n is in use by the library */
u16 data_arm7_03807a78;
/* also declared in src/arm7/sdk/os/tick_is_running_arm7.c */
u16 data_arm7_03807a7c;
/* also declared in src/arm7/sdk/os/tick_on_timer0_overflow_arm7.c */
u32 gOsiNeedResetTimer_Arm7;
/* also declared in src/arm7/sdk/os/tick_on_timer0_overflow_arm7.c and 1 more */
vu64 gOsiTickCounter_Arm7;

/*
 * ARM7 copy of OS_InitTick. Once only: zeroes the tick counter and starts
 * timer 0, counting up with an interrupt on overflow.
 */
void Tick_StartTimer0_Arm7(void) {
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
  Irq_InstallHandlers_Arm7(IRQ_MASK_TIMER0,
                           (OSIrqFunction)Tick_OnTimer0Overflow_Arm7);
  Irq_SetEnableBits_Arm7(IRQ_MASK_TIMER0);
  gOsiNeedResetTimer_Arm7 = 0;
}
