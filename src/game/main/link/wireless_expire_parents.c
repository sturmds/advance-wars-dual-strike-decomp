#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern u16 gWirelessParentTimers[4];
extern u8 gWirelessParentLinkLevels[4];
extern WMBssDesc *data_0229c8cc[4];             /* parents found by the scan */

void Mem_CopyBytes(const void *src, void *dst, u32 size);

/* While scanning, counts down each found parent's timer (data_0215fe84->unk_34
 * entries) and removes the parents whose timer reaches 0, moving later entries
 * down. */
void Wireless_ExpireParents(void)
{
    int i;
    int j;

    if (data_0215fe84->unk_14 == 0) {
        return;
    }
    for (i = 0; i < data_0215fe84->unk_34; i++) {
        gWirelessParentTimers[i]--;
        if (gWirelessParentTimers[i] == 0) {
            if (i + 1 != data_0215fe84->unk_34) {
                for (j = i; j < data_0215fe84->unk_34 - 1; j++) {
                    Mem_CopyBytes(data_0229c8cc[j + 1], data_0229c8cc[j], sizeof(WMBssDesc));
                    gWirelessParentTimers[j] = gWirelessParentTimers[j + 1];
                    gWirelessParentLinkLevels[j] = gWirelessParentLinkLevels[j + 1];
                }
            }
            data_0215fe84->unk_34--;
            i--;
        }
    }
}
