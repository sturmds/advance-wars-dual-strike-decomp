#include "sdk/wl.h"

void func_arm7_027cb2d0(u32 station, u32 rates)
{
    WifiStation_Entry(station)->rates = rates;
}
