#include "sdk/fs.h"

BOOL FS_LoadOverlayInfo(FSOverlayInfo *info, int target, u32 overlayId);
void FS_StartOverlay(FSOverlayInfo *info);

/* Reads an overlay into memory, then runs its static initialisers. */
BOOL FS_LoadOverlay(int target, u32 overlayId)
{
    FSOverlayInfo info;

    if (!FS_LoadOverlayInfo(&info, target, overlayId) || !FS_LoadOverlayImage(&info)) {
        return FALSE;
    }
    FS_StartOverlay(&info);
    return TRUE;
}
