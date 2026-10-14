#include "sdk/wl.h"
#include "sdk/mi.h"

void Wifi_ClearSessionArea(u32 value, int limit) {
  // The clear of the link area below wipes unk_b4; keep it.
  void* saved = WIFI_DRIVER_STATE->state.unk_b4;
  volatile u16 zeroHead = 0;
  volatile u16 zeroLink;

  Mem_FillHalfwords_Arm7(
      zeroHead, &WIFI_DRIVER_STATE->limits.unk_f0,
      offsetof(WlWork, unk_318) - offsetof(WlWork, limits.unk_f0));
  zeroLink = 0;
  Mem_FillHalfwords_Arm7(zeroLink, WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE),
                         sizeof(WlLink));
  WIFI_DRIVER_STATE->limits.unk_f0 = value;
  WIFI_DRIVER_STATE->limits.unk_f4 = (u16)limit;
  WIFI_DRIVER_STATE->limits.unk_f6 = (u16)limit;
  WIFI_DRIVER_STATE->state.unk_b4 = saved;
}
