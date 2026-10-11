#include "sdk/fs.h"

BOOL func_021133a0(FSOverlayInfo *info, int target, u32 overlayId);
BOOL func_02113000(FSOverlayInfo *info);

BOOL FS_UnloadOverlay(int target, u32 overlayId)
{
    FSOverlayInfo info;

    if (!func_021133a0(&info, target, overlayId) || !func_02113000(&info)) {
        return FALSE;
    }
    return TRUE;
}
