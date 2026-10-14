#include "types.h"
#include "game/wireless_parent_info.h"

void Wireless_OnParentParamSet(void* event);
int WM_SetParentParameter(void (*callback)(void* event), void* param);
extern WirelessParentInfo* data_0229c8a0;

BOOL func_0207909c(void) {
  return WM_SetParentParameter(Wireless_OnParentParamSet, data_0229c8a0) == 2;
}
