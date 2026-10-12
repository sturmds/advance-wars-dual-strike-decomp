#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_event.h"

/* Callback of the mode request in Wireless_SetEntry: stores mode 0 on success. */
void Wireless_OnSetMode0(WirelessEvent* event) {
  if (event->errorCode == 0) {
    data_0215fe84->unk_18 = 0;
  }
}
