#include "types.h"
#include "sdk/os.h"

#define REG_TM0CNT_L (*(vu16*)0x04000100)
#define REG_TM0CNT_H (*(vu16*)0x04000102)

extern vu64 gOsiTickCounter;
extern u32 gOsiNeedResetTimer;
void Timer_InstallIrqSlot(int timer, void (*handler)(void*), void* arg);

/* Timer 0 overflow handler: counts one more overflow and arms itself for the next. */
void OSi_CountUpTick(void* arg) {
  gOsiTickCounter++;
  if (gOsiNeedResetTimer != 0) {
    REG_TM0CNT_H = 0;
    REG_TM0CNT_L = 0;
    REG_TM0CNT_H = 0xc1;
    gOsiNeedResetTimer = 0;
  }
  Timer_InstallIrqSlot(0, OSi_CountUpTick, NULL);
}
