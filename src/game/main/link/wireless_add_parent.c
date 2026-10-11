#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"
#include "sdk/wm.h"

extern WMBssDesc *data_0229c8a8;
extern u16 gWirelessParentTimers[4];
extern u8 gWirelessParentLinkLevels[4];
extern WMBssDesc *data_0229c8cc[4];

void Mem_CopyBytes(const void *src, void *dst, u32 size);

/* Records the parent in the scan buffer (data_0229c8a8). A parent already in
 * the list gets its entry refreshed and 180 frames on its timer; a new one is
 * added only if it takes entries, its unk_17 matches ours, it has a free child
 * slot and the list has room (4). The beacon's user data has the same layout
 * as data_0229c910. */
void Wireless_AddParent(u8 linkLevel)
{
    int i;
    Unk_0229c910 *info;
    int foundEnd;                      /* needed to match: not slotEnd */
    int j;
    int slotEnd;
    int slot;

    for (i = 0; i < data_0215fe84->unk_34; i++) {
        for (j = 0; j < 6; j++) {
            if (data_0229c8a8->bssid[j] != data_0229c8cc[i]->bssid[j]) {
                break;
            }
        }
        if (j == 6) {
            goto found;
        }
    }

    if (!(data_0229c8a8->gameInfo.attribute & WM_ATTR_FLAG_ENTRY)) {
        return;
    }
    info = (Unk_0229c910 *)data_0229c8a8->gameInfo.userGameInfo;
    if (data_0215fe84->unk_35 != info->unk_17) {
        return;
    }
    slotEnd = info->unk_16 + 1;
    for (slot = 1; slot < slotEnd; slot++) {
        if (!(info->unk_14 & (1 << slot))) {
            break;
        }
    }
    if (slot == slotEnd) {
        return;
    }
    if (data_0215fe84->unk_34 >= 4) {
        return;
    }
    Mem_CopyBytes(data_0229c8a8, data_0229c8cc[data_0215fe84->unk_34], sizeof(WMBssDesc));
    gWirelessParentTimers[data_0215fe84->unk_34] = 180;
    gWirelessParentLinkLevels[data_0215fe84->unk_34] = linkLevel;
    data_0215fe84->unk_34++;
    return;

found:
    /* a timer of 1: Wireless_ExpireParents removes a closed or full parent */
    Mem_CopyBytes(data_0229c8a8, data_0229c8cc[i], sizeof(WMBssDesc));
    gWirelessParentTimers[i] = 180;
    gWirelessParentLinkLevels[i] = linkLevel;
    if (!(data_0229c8a8->gameInfo.attribute & WM_ATTR_FLAG_ENTRY)) {
        gWirelessParentTimers[i] = 1;
    }
    info = (Unk_0229c910 *)data_0229c8a8->gameInfo.userGameInfo;
    if (data_0215fe84->unk_35 != info->unk_17) {
        gWirelessParentTimers[i] = 1;
    }
    foundEnd = info->unk_16 + 1;
    for (slot = 1; slot < foundEnd; slot++) {
        if (!(info->unk_14 & (1 << slot))) {
            break;
        }
    }
    if (slot == foundEnd) {
        gWirelessParentTimers[i] = 1;
    }
}
