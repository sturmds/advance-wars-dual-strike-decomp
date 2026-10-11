#include "sdk/fs.h"

BOOL FS_LoadOverlayInfo(FSOverlayInfo *info, int target, u32 overlayId);
BOOL func_02113000(FSOverlayInfo *info);

BOOL FS_UnloadOverlay(int target, u32 overlayId)
{
    FSOverlayInfo info;

    if (!FS_LoadOverlayInfo(&info, target, overlayId) || !func_02113000(&info)) {
        return FALSE;
    }
    return TRUE;
}
