#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c910.h"
#include "sdk/wm.h"
#include "game/wireless.h"
#include "sdk/os_common.h"

extern u16 gWirelessParentTimers[];
extern u8 gWirelessParentLinkLevels[];

void Mem_CopyBytes(const void* src, void* dst, u32 size);

/* Records the parent in the scan buffer (gWirelessScanBuffer). A parent already in
 * the list gets its entry refreshed and 180 frames on its timer; a new one is
 * added only if it takes entries, its unk_17 matches ours, it has a free child
 * slot and the list has room (4). The beacon's user data has the same layout
 * as data_0229c910. */
void Wireless_AddParent(u8 linkLevel) {
  int i;
  Unk_0229c910* info;
  int foundEnd; /* needed to match: not slotEnd */
  int j;
  int slotEnd;
  int slot;

  for (i = 0; i < data_0215fe84->parentCount; i++) {
    for (j = 0; j < MAC_ADDRESS_SIZE; j++) {
      if (gWirelessScanBuffer->bssid[j] != gWirelessParents[i]->bssid[j]) {
        break;
      }
    }
    if (j == 6) {
      goto found;
    }
  }

  if (!(gWirelessScanBuffer->gameInfo.attribute & WM_ATTR_FLAG_ENTRY)) {
    return;
  }
  info = (Unk_0229c910*)gWirelessScanBuffer->gameInfo.userGameInfo;
  if (data_0215fe84->unk_35 != info->unk_17) {
    return;
  }
  slotEnd = info->maxChildren + 1;
  for (slot = 1; slot < slotEnd; slot++) {
    if (!(info->usedAids & (1 << slot))) {
      break;
    }
  }
  if (slot == slotEnd) {
    return;
  }
  if (data_0215fe84->parentCount >= 4) {
    return;
  }
  Mem_CopyBytes(gWirelessScanBuffer,
                gWirelessParents[data_0215fe84->parentCount],
                sizeof(WMBssDesc));
  gWirelessParentTimers[data_0215fe84->parentCount] = 180;
  gWirelessParentLinkLevels[data_0215fe84->parentCount] = linkLevel;
  data_0215fe84->parentCount++;
  return;

found:
  /* a timer of 1: Wireless_ExpireParents removes a closed or full parent */
  Mem_CopyBytes(gWirelessScanBuffer, gWirelessParents[i], sizeof(WMBssDesc));
  gWirelessParentTimers[i] = 180;
  gWirelessParentLinkLevels[i] = linkLevel;
  if (!(gWirelessScanBuffer->gameInfo.attribute & WM_ATTR_FLAG_ENTRY)) {
    gWirelessParentTimers[i] = 1;
  }
  info = (Unk_0229c910*)gWirelessScanBuffer->gameInfo.userGameInfo;
  if (data_0215fe84->unk_35 != info->unk_17) {
    gWirelessParentTimers[i] = 1;
  }
  foundEnd = info->maxChildren + 1;
  for (slot = 1; slot < foundEnd; slot++) {
    if (!(info->usedAids & (1 << slot))) {
      break;
    }
  }
  if (slot == foundEnd) {
    gWirelessParentTimers[i] = 1;
  }
}
