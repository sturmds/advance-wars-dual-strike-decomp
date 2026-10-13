#include "sdk/pm.h"
#include "sdk/pxi.h"

static u16 gPMInitialized;
volatile int gPMSleepRequestAck;
void* data_022ab8f8;
u32 data_022ab8f4;
void* data_022ab8f0;
volatile int gPMSleepAck;
PowerCommandState gPMWork;
static u8 data_022ab90c[20];
PowerChipReadSlot gPMRegisterReads[POWER_CHIP_READ_SLOTS];

void func_0210e0c0(void* mutex);
void Power_OnArm7Reply(u32 tag, u32 data, int err);

/* Starts the PM service: waits for the ARM7 on the PM PXI channel and listens for its replies. */
void PM_Init(void) {
  int i;

  if (gPMInitialized != 0) {
    return;
  }
  gPMInitialized = 1;
  gPMWork.lock = 0;
  gPMWork.callback = NULL;
  PXI_Init();
  while (!PXI_IsCallbackReady(PXI_FIFO_TAG_PM, 1)) {
  }
  PXI_SetFifoRecvCallback(PXI_FIFO_TAG_PM, Power_OnArm7Reply);
  for (i = 0; i < POWER_CHIP_READ_SLOTS; i++) {
    gPMRegisterReads[i].done = 0;
  }
  func_0210e0c0(data_022ab90c);
  data_022ab8f4 = *SHARED_FRAME_COUNTER;
}
