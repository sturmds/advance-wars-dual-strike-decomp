#include "sdk/mic.h"
#include "sdk/os_arm7.h"

void func_arm7_03805904(void);

/* Timer 3 interrupt handler during auto sampling: takes one sample, then
 * marks the interrupt as handled. */
void func_arm7_03805a6c(void) {
  func_arm7_03805904();
  OS7_IRQ_CHECK_FLAGS |= IRQ_MASK_TIMER3;
  *REG_IF = IRQ_MASK_TIMER3;
}
