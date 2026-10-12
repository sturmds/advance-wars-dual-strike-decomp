#include "sdk/mic.h"

/* Resets the microphone driver: idle state, no partial request, timer 3 stopped. */
void func_arm7_0380607c(void) {
  int i;

  data_arm7_0380af74.status = MIC_STATUS_READY;
  for (i = 0; i < MIC_PXI_WORDS; i++) {
    data_arm7_0380af74.command[i] = 0;
  }
  REG_TM3CNT_H &= ~TIMER_ENABLE;
}
