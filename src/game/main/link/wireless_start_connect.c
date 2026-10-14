#include "types.h"
#include "game/wireless.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern WMBssDesc* data_0229c888;

void Wireless_OnConnectEvent(void* event);
void Wireless_SetPlayerName(u8 aid, u8* ssid);
void Wireless_ClearPlayerName(u8 aid);
int Wireless_RequestJoinParent(void (*callback)(void* event), WMBssDesc* parent,
                               u8* ssid, int powerSave, int authMode);

/* Child side: forgets the data kept for every aid, stores the chosen parent's
 * data for aid 0, and requests a connection to it. True when the request
 * started; false sets Unk_0215fe84.unk_24. */
BOOL Wireless_StartConnect(void) {
  int aid;

  for (aid = 0; aid < 8; aid++) {
    Wireless_ClearPlayerName(aid);
  }
  Wireless_SetPlayerName(0, data_0229c888->gameInfo.userGameInfo);
  if (Wireless_RequestJoinParent(Wireless_OnConnectEvent, data_0229c888,
                                 data_0229c8f8, 1, 0) != 2) {
    data_0215fe84->unk_24 = 1;
    return FALSE;
  }
  return TRUE;
}
