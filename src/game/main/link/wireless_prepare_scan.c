#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern WMBssDesc* gWirelessScanBuffer;
extern WMScanParam* data_0229c8b4;

BOOL Wireless_NextScanChannel(u16* channel);
u16 WM_GetDispersionScanPeriod(void);

/* Moves to the next scan channel and fills the scan request for it. False
 * when no channel is allowed. */
BOOL Wireless_PrepareScan(void) {
  if (!Wireless_NextScanChannel(&data_0215fe84->channel)) {
    return FALSE;
  }
  data_0229c8b4->scanBuf = gWirelessScanBuffer;
  data_0229c8b4->channel = data_0215fe84->channel;
  data_0229c8b4->maxChannelTime = WM_GetDispersionScanPeriod();
  data_0229c8b4->bssid[0] = 0xff;
  data_0229c8b4->bssid[1] = 0xff;
  data_0229c8b4->bssid[2] = 0xff;
  data_0229c8b4->bssid[3] = 0xff;
  data_0229c8b4->bssid[4] = 0xff;
  data_0229c8b4->bssid[5] = 0xff;
  return TRUE;
}
