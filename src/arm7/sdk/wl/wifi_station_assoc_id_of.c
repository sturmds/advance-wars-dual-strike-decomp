#include "sdk/wl.h"

u16 WifiStation_AssocIdOf(u32 station)
{
    return WifiStation_Entry(station)->aid;
}
