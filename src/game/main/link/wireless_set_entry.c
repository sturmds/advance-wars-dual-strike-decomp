#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_event.h"
#include "sdk/wm.h"

extern u32 gWirelessState;
void Wireless_OnSetMode0(WirelessEvent* event);
void Wireless_OnSetMode1(WirelessEvent* event);

/* Switches whether the parent takes new children (mode 1) or not (mode 0),
 * when the link is up and the mode changes. */
void Wireless_SetEntry(int mode) {
  if (gWirelessState != 4) {
    return;
  }
  if (mode == data_0215fe84->unk_18) {
    return;
  }
  if (mode == 1) {
    WM_SetEntry((WMCallbackFunc)Wireless_OnSetMode1, mode);
    return;
  }
  WM_SetEntry((WMCallbackFunc)Wireless_OnSetMode0, mode);
}
