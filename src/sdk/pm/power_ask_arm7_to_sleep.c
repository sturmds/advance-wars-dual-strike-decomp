#include "sdk/pm.h"

extern volatile int gPMSleepAck;
extern volatile int gPMSleepRequestAck;

BOOL Power_TryTakeCommandSlot(void);
void PMi_SendPxiData(u32 data);

/* Asks the ARM7 to prepare for sleep, waits for its answer, switches the LCDs off, then sends the wake-up conditions. */
int Power_AskArm7ToSleep(u32 trigger, u32 keys) {
  if (!Power_TryTakeCommandSlot()) {
    return 1;
  }
  gPMSleepAck = 0;
  PMi_SendPxiData(0x03006000);
  while (gPMSleepAck == 0) {
  }
  gPMSleepAck = 0;
  gPMSleepRequestAck = 0;
  Power_SwitchLcds(0, 2, FALSE, FALSE);
  PMi_SendPxiData(0x02006100 | (trigger & 0xff));
  PMi_SendPxiData((keys & 0xffff) | 0x01010000);
  return 0;
}
