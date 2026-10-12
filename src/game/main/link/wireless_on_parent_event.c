#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"
#include "sdk/wm.h"

extern u32 data_0229c880;

int func_02079180(void);
int Wireless_StartMp(void);
void Wireless_CountChildren(void);
void Wireless_SetPlayerName(u8 aid, u8* ssid);
void Wireless_ClearPlayerName(u8 aid);
void Wireless_SetGameInfo(void);

/* Parent callback: tracks which children are connected and passes each
 * connect and disconnect on to the link code. */
void Wireless_OnParentEvent(WMStartParentCallback* cb) {
  u8* ssid;

  if (cb->errcode == 0) {
    switch (cb->state) {
      case WM_STATECODE_PARENT_START:
        data_0215fe84->unk_06 = 0;
        switch (data_0229c880) {
          case 0:
            func_02079180();
            return;
          case 4:
            if (Wireless_StartMp()) {
              return;
            }
            func_02079180();
            return;
          case 3:
          case 5:
            data_0229c880 = 3;
            func_02079180();
            return;
        }
        break;
      case WM_STATECODE_BEACON_SENT:
        break;
      case WM_STATECODE_CONNECTED:
        data_0215fe84->childAids |= 1 << cb->aid;
        Wireless_CountChildren();
        ssid = cb->ssid;
        Wireless_SetPlayerName(cb->aid, ssid);
        data_0229c910.usedAids = data_0215fe84->childAids | 1;
        Wireless_SetGameInfo();
        Wireless_SetPlayerName(cb->aid, ssid);
        data_0215fe84->childrenChanged = 1;
        break;
      case WM_STATECODE_DISCONNECTED:
        /* needed to match: ~(1 << aid) gives mvn instead of eor */
        data_0215fe84->childAids &= ~0 ^ (1 << cb->aid);
        Wireless_CountChildren();
        data_0229c910.usedAids = data_0215fe84->childAids | 1;
        Wireless_SetGameInfo();
        Wireless_ClearPlayerName(cb->aid);
        data_0215fe84->childrenChanged = 1;
        break;
    }
  } else {
    data_0229c880 = 4;
    func_02079180();
  }
}
