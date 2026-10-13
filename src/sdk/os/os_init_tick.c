#include "types.h"
#include "sdk/os.h"

#define REG_TM0CNT_L (*(vu16*)0x04000100)
#define REG_TM0CNT_H (*(vu16*)0x04000102)
#define IRQ_MASK_TIMER0 0x8

static u16 gOsTickInitialized;
u32 gOsiNeedResetTimer;
vu64 gOsiTickCounter;
void OSi_SetTimerReserved(int timer);
void OSi_CountUpTick(void* arg);

/* Once only: zeroes the tick counter and starts timer 0, counting up with an interrupt on overflow. */
void OS_InitTick(void) {
  if (gOsTickInitialized != 0) {
    return;
  }
  gOsTickInitialized = 1;
  OSi_SetTimerReserved(0);
  gOsiTickCounter = 0;
  REG_TM0CNT_H = 0;
  REG_TM0CNT_L = 0;
  REG_TM0CNT_H = 0xc1;
  OS_SetIrqFunction(IRQ_MASK_TIMER0, (OSIrqFunction)OSi_CountUpTick);
  OS_EnableIrqMask(IRQ_MASK_TIMER0);
  gOsiNeedResetTimer = 0;
}
