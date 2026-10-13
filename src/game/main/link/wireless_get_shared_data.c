#include "types.h"
#include "sdk/wm.h"
#include "game/wireless.h"

u16* Wireless_GetSharedData(u16 aid) {
  return WM_GetSharedDataAddress(data_0229c8ac, data_0229c8b0, aid);
}
