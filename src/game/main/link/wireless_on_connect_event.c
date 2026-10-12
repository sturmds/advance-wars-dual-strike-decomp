#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern u32 data_0229c880;
extern u32 gWirelessState;
extern u8 data_0229c8f8[];

int func_02079180(void);
void Wireless_SetPlayerName(u8 aid, u8* ssid);
BOOL Wireless_StartMp(void);

/* Lost beacons (counted while Unk_0215fe84.unk_18 is 0) after which the
 * child gives up. */
#define MAX_BEACONS_LOST 5

/* Child side: callback of the connect request in Wireless_StartConnect. In
 * link states 0, 3 and 4 it only calls func_02079180. In state 5: once
 * connected it keeps its aid, stores the ssid it connected with
 * (data_0229c8f8) as that aid's name and starts MP (unless gWirelessState
 * is 5); after MAX_BEACONS_LOST lost beacons it sets gWirelessState to 1;
 * on a disconnect it goes to state 3 and sets gWirelessState to 2. Any other state code goes back to state 0. Errors go
 * to state 3 and set unk_24. */
void Wireless_OnConnectEvent(WMStartConnectCallback* cb) {
  if (cb->errcode == WM_ERRCODE_SUCCESS) {
    switch (data_0229c880) {
      case 0:
      case 3:
      case 4:
        func_02079180();
        return;
      case 5:
        switch (cb->state) {
          case WM_STATECODE_CONNECT_START:
            break;
          case WM_STATECODE_CONNECTED:
            data_0215fe84->unk_06 = cb->aid;
            Wireless_SetPlayerName(cb->aid, data_0229c8f8);
            if (gWirelessState == 5) {
              return;
            }
            if (Wireless_StartMp()) {
              return;
            }
            data_0229c880 = 3;
            data_0215fe84->unk_24 = 1;
            func_02079180();
            return;
          case WM_STATECODE_BEACON_LOST:
            if (data_0215fe84->unk_18 != 0) {
              return;
            }
            data_0215fe84->beaconLostCount++;
            if (data_0215fe84->beaconLostCount < MAX_BEACONS_LOST) {
              return;
            }
            gWirelessState = 1;
            func_02079180();
            return;
          case WM_STATECODE_DISCONNECTED:
            data_0229c880 = 3;
            if (gWirelessState == 5) {
              data_0215fe84->unk_28 = 1;
            }
            if (gWirelessState == 2) {
              return;
            }
            gWirelessState = 2;
            func_02079180();
            return;
          default:
            data_0229c880 = 0;
            func_02079180();
            return;
        }
    }
  } else {
    data_0229c880 = 3;
    data_0215fe84->unk_24 = 1;
    func_02079180();
  }
}
