#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern WMBssDesc *gWirelessParents[4];

void Mem_ZeroUnits(u32 value, void *dst, u32 size, u32 unitBits);

/* Empties the list of scanned parents. */
void Wireless_ClearParents(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        Mem_ZeroUnits(0, gWirelessParents[i], sizeof(WMBssDesc), 8);
    }
    data_0215fe84->parentCount = 0;
}
