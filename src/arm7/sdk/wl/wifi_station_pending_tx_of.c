#include "sdk/wl.h"

u16 WifiStation_PendingTxOf(u32 station)
{
    return WifiStation_Entry(station)->queuedFrames;
}
