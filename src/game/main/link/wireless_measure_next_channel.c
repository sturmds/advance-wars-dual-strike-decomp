#include "types.h"
#include "game/Unk_0215fe88.h"
#include "sdk/wm.h"

int WM_MeasureChannel(void (*callback)(WMMeasureChannelCallback *cb), int ccaMode, int edThreshold, int channel, int measureTime);
void Wireless_OnChannelMeasured(WMMeasureChannelCallback *cb);

/* Moves to the next allowed channel and starts measuring it. False when
 * there is no channel left or the request does not start. */
BOOL Wireless_MeasureNextChannel(void)
{
    int channel;

    data_0215fe88->channel++;
    channel = data_0215fe88->channel;
    if (channel <= 14) {
        do {
            if (data_0215fe88->allowedChannels & (1 << (channel - 1))) {
                break;
            }
            channel++;
        } while (channel <= 14);
    }
    data_0215fe88->channel = channel;
    if (data_0215fe88->channel >= 15) {
        return FALSE;
    }
    if (WM_MeasureChannel(Wireless_OnChannelMeasured, 3, 17, data_0215fe88->channel, 30) != 2) {
        data_0215fe88->bestChannel = 0;
        return FALSE;
    }
    return TRUE;
}
