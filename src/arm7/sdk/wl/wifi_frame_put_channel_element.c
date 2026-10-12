#include "sdk/wl.h"

void WL_WriteByte(void *addr, u32 value);

// Writes the DS Parameter Set element (id 3, one byte: the channel in
// WlState.unk_92) and returns its size.
u32 WifiFrame_PutChannelElement(u8 *dest)
{
    WL_WriteByte(dest, 3);
    WL_WriteByte(dest + 1, 1);
    WL_WriteByte(dest + 2, WIFI_DRIVER_STATE->state.unk_92 & 0xff);
    return 3;
}
