#include "sdk/wl.h"

void func_arm7_027c9560(void);

int WifiParam_StoreRates(const WlRateSet* rates) {
  // WlState.unk_78 and unk_7a hold a WlRateSet; the pointer is needed to match.
  WlRateSet* rateSet = (WlRateSet*)&WIFI_DRIVER_STATE->state.unk_78;

  rateSet->basic = rates->basic;
  rateSet->supported = rates->supported | rates->basic;
  func_arm7_027c9560();
  return 0;
}
