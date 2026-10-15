#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"
#include "game/wireless.h"

extern u16 gWirelessParentTimers[];

void Mem_CopyBytes(const void* src, void* dst, u32 size);

/* While scanning, counts down each found parent's timer (data_0215fe84->parentCount
 * entries) and removes the parents whose timer reaches 0, moving later entries
 * down. */
void Wireless_ExpireParents(void) {
  int i;
  int j;

  if (data_0215fe84->scanning == 0) {
    return;
  }
  for (i = 0; i < data_0215fe84->parentCount; i++) {
    gWirelessParentTimers[i]--;
    if (gWirelessParentTimers[i] == 0) {
      if (i + 1 != data_0215fe84->parentCount) {
        for (j = i; j < data_0215fe84->parentCount - 1; j++) {
          Mem_CopyBytes(gWirelessParents[j + 1], gWirelessParents[j],
                        sizeof(WMBssDesc));
          gWirelessParentTimers[j] = gWirelessParentTimers[j + 1];
          gWirelessParentLinkLevels[j] = gWirelessParentLinkLevels[j + 1];
        }
      }
      data_0215fe84->parentCount--;
      i--;
    }
  }
}
