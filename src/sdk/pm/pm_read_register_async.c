#include "sdk/pm.h"

extern PowerCommandState gPMWork;
extern PowerChipReadSlot gPMRegisterReads[POWER_CHIP_READ_SLOTS];

BOOL Power_TryTakeCommandSlot(void);
void PMi_SendPxiData(u32 data);

/* Asks the ARM7 for PM register `reg`; the reply stores it in *data. Returns 1 when another command is in flight. */
int PMi_ReadRegisterAsync(u32 reg, u16* data, PMCallback callback, u32* arg) {
  if (!Power_TryTakeCommandSlot()) {
    return 1;
  }
  gPMRegisterReads[reg].done = 0;
  gPMWork.callback = callback;
  gPMWork.arg = arg;
  gPMRegisterReads[reg].data = data;
  PMi_SendPxiData(0x03006500 | (reg & 0xff));
  return 0;
}
