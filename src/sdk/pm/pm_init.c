#include "sdk/pm.h"

extern u16 gPMInitialized;
extern PowerCommandState gPMWork;
extern u8 data_022ab90c[20];
extern PowerChipReadSlot gPMRegisterReads[POWER_CHIP_READ_SLOTS];
extern u32 data_022ab8f4;

void func_02110e58(void);
BOOL PXI_IsCallbackReady(int tag, int proc);
void PXI_SetFifoRecvCallback(int tag, void (*callback)(int tag, u32 data, int err));
void func_0210e0c0(void *mutex);
void Power_OnArm7Reply(int tag, u32 data, int err);

/* Starts the PM service: waits for the ARM7 on the PM PXI channel and listens for its replies. */
void PM_Init(void)
{
    int i;

    if (gPMInitialized != 0) {
        return;
    }
    gPMInitialized = 1;
    gPMWork.lock = 0;
    gPMWork.callback = NULL;
    func_02110e58();
    while (!PXI_IsCallbackReady(PXI_FIFO_TAG_PM, 1)) {
    }
    PXI_SetFifoRecvCallback(PXI_FIFO_TAG_PM, Power_OnArm7Reply);
    for (i = 0; i < POWER_CHIP_READ_SLOTS; i++) {
        gPMRegisterReads[i].done = 0;
    }
    func_0210e0c0(data_022ab90c);
    data_022ab8f4 = *SHARED_FRAME_COUNTER;
}
