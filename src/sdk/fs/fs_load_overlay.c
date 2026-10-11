#include "sdk/fs.h"

BOOL func_021133a0(FSOverlayInfo *info, int target, u32 overlayId);
void FS_StartOverlay(FSOverlayInfo *info);

/* Reads an overlay into memory, then runs its static initialisers. */
BOOL FS_LoadOverlay(int target, u32 overlayId)
{
    FSOverlayInfo info;

    if (!func_021133a0(&info, target, overlayId) || !FS_LoadOverlayImage(&info)) {
        return FALSE;
    }
    FS_StartOverlay(&info);
    return TRUE;
}
