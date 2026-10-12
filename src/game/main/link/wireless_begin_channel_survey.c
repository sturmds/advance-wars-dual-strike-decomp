#include "types.h"
#include "game/Unk_0215fe88.h"

u16 WM_GetAllowedChannel(void);
BOOL Wireless_MeasureNextChannel(void);

/* Reads the allowed channels, resets the survey and measures the first one.
 * False when no channel is allowed or the measurement does not start. */
BOOL Wireless_BeginChannelSurvey(void) {
  data_0215fe88->allowedChannels = WM_GetAllowedChannel();
  if (data_0215fe88->allowedChannels == 0) {
    return FALSE;
  }
  data_0215fe88->bestChannel = 0;
  data_0215fe88->channel = 0;
  data_0215fe88->bestBusyRatio = 101;
  return Wireless_MeasureNextChannel() != 0;
}
