#include "types.h"
#include "sdk/wm.h"

extern WMDataSharingInfo* data_0229c8ac;
extern WMDataSet* data_0229c8b0;

u16* Wireless_GetSharedData(u16 aid) {
  return WM_GetSharedDataAddress(data_0229c8ac, data_0229c8b0, aid);
}
