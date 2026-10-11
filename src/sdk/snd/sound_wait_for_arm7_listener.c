#include "sdk/snd.h"
#include "sdk/os.h"

typedef void (*PxiCallback)(int tag, u32 data, int err);

/* PXI_SetFifoRecvCallback, PXI_IsCallbackReady (guessed names). */
void func_02111064(int tag, PxiCallback callback);
int func_0211103c(int tag, int peer);
int func_0210dc64(void);

void Sound_WaitForArm7Listener(void)
{
    func_02111064(SOUND_FIFO_TAG, SoundAlarm_OnArm7Message);
    if (func_0210dc64() != 0) {
        return;
    }
    if (func_0211103c(SOUND_FIFO_TAG, 1) != 0) {
        return;
    }
    do {
        OS_SpinWait(100);
    } while (func_0211103c(SOUND_FIFO_TAG, 1) == 0);
}
