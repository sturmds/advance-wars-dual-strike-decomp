#include "types.h"
#include "sdk/os.h"

#define REG_TM0CNT_L (*(vu16*)0x04000100)
#define IRQ_MASK_TIMER0 0x8

extern vu64 gOsiTickCounter;

/* 64-bit tick count: timer 0 overflow count above its 16-bit counter. */
u64 OS_GetTick(void) {
  int irqState;
  vu16 counter;
  vu64 overflows;

  irqState = OS_DisableInterrupts();
  counter = REG_TM0CNT_L;
  overflows = gOsiTickCounter & 0x0000ffffffffffffULL;

  /* The overflow interrupt is pending but has not been counted yet. */
  if ((*REG_IF & IRQ_MASK_TIMER0) && !(counter & 0x8000)) {
    overflows++;
  }
  OS_RestoreInterrupts(irqState);
  return (overflows << 16) | counter;
}
