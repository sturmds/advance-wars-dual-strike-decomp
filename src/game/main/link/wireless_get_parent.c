#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern WMBssDesc *data_0229c8cc[4];

/* Returns a scanned parent by list index, or 0 past the end of the list. */
WMBssDesc *Wireless_GetParent(u32 index)
{
    if (index >= data_0215fe84->unk_34) {
        return 0;
    }
    return data_0229c8cc[index];
}
