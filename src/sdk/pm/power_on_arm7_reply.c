#include "sdk/pm.h"

extern PowerCommandState gPMWork;
extern PowerChipReadSlot gPMRegisterReads[POWER_CHIP_READ_SLOTS];
extern volatile int gPMSleepAck;
extern volatile int gPMSleepRequestAck;

void Power_ReleaseAndReport(u32 result);

/* PXI callback for tag 8: one reply word from the ARM7 (command in bits 8-14, value in bits 0-7). */
void Power_OnArm7Reply(u32 tag, u32 data, int err) {
  u16 command;
  u16 value;
  u32 result;

  if (err) {
    Power_ReleaseAndReport(2);
    return;
  }
  command = (data & 0x7f00) >> 8;
  value = data & 0xff;
  result = value;
  if (command >= 0x70 && command <= 0x73) {
    int reg = command - 0x70;
    u16 reading = value & 0xff;

    if (gPMRegisterReads[reg].data != NULL) {
      *gPMRegisterReads[reg].data = reading;
    }
    gPMRegisterReads[reg].done = 1;
    result = 0;
  } else if (command == 0x60) {
    gPMSleepAck = 1;
  } else if (command == 0x62) {
    gPMSleepRequestAck = 1;
  } else if (command == 0x67) {
    if (gPMWork.unk_0c != NULL) {
      *gPMWork.unk_0c = value;
    }
    result = 0;
  }
  Power_ReleaseAndReport(result);
}
