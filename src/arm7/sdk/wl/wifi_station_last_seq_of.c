#include "sdk/wl.h"

u16 WifiStation_LastSeqOf(u32 station)
{
    return WifiStation_Entry(station)->lastSequence;
}
