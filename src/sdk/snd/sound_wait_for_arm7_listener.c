#include "sdk/snd.h"
#include "sdk/os.h"

typedef void (*PxiCallback)(int tag, u32 data, int err);

/* PXI_SetFifoRecvCallback, PXI_IsCallbackReady (guessed names). */
void PXI_SetFifoRecvCallback(int tag, PxiCallback callback);
int PXI_IsCallbackReady(int tag, int peer);
int func_0210dc64(void);

void Sound_WaitForArm7Listener(void) {
  PXI_SetFifoRecvCallback(SOUND_FIFO_TAG, SoundAlarm_OnArm7Message);
  if (func_0210dc64() != 0) {
    return;
  }
  if (PXI_IsCallbackReady(SOUND_FIFO_TAG, 1) != 0) {
    return;
  }
  do {
    OS_SpinWait(100);
  } while (PXI_IsCallbackReady(SOUND_FIFO_TAG, 1) == 0);
}
