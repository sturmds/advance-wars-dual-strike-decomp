#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern WMBssDesc *gWirelessParents[4];
extern WMBssDesc *data_0229c888;

void Mem_CopyBytes(const void *src, void *dst, u32 size);

/* Child side: copies the scanned parent at index to data_0229c888, the parent
 * Wireless_StartConnect connects to. FALSE when index is past the list. */
BOOL Wireless_SelectParent(u32 index)
{
    if (data_0215fe84->parentCount <= index) {
        return FALSE;
    }
    Mem_CopyBytes(gWirelessParents[index], data_0229c888, sizeof(WMBssDesc));
    return TRUE;
}
