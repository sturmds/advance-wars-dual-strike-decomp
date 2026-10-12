#include "sdk/wl.h"

void WifiStation_StoreRates(u32 station, u32 rates)
{
    WifiStation_Entry(station)->rates = rates;
}
