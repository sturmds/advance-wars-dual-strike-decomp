#include "sdk/wl.h"

u32 WifiParam_ChannelAllowed(u32 bit) {
  return (1 << bit) & WIFI_DRIVER_STATE->state.unk_00;
}
