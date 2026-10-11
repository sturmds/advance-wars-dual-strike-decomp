#include "types.h"
#include "game/Unk_0215fe88.h"

u16 func_02117368(void);
BOOL func_02079e28(void);

/* Reads the allowed channels, resets the survey and measures the first one.
 * False when no channel is allowed or the measurement does not start. */
BOOL Wireless_BeginChannelSurvey(void)
{
    data_0215fe88->allowedChannels = func_02117368();
    if (data_0215fe88->allowedChannels == 0) {
        return FALSE;
    }
    data_0215fe88->bestChannel = 0;
    data_0215fe88->channel = 0;
    data_0215fe88->bestBusyRatio = 101;
    return func_02079e28() != 0;
}
