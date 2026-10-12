#include "sdk/snd.h"
#include "sdk/snd_hw.h"

void SndArm7_UpdateSharedWork(void)
{
    u16 channelStatus = 0;
    u16 captureStatus = 0;
    int i;

    if (data_arm7_0380808c == NULL) {
        return;
    }
    for (i = 0; i < AUDIO_CH_TOTAL; i++) {
        if (SndHw_IsChannelBusy(i)) {
            channelStatus |= 1 << i;
        }
    }
    if (SND_IsCaptureActive(0)) {
        captureStatus |= 1;
    }
    if (SND_IsCaptureActive(1)) {
        captureStatus |= 2;
    }
    data_arm7_0380808c->channelStatus = channelStatus;
    data_arm7_0380808c->captureStatus = captureStatus;
}
