#include "sdk/fs.h"

BOOL func_021133a0(FSOverlayInfo *info, int target, u32 overlayId);
BOOL func_02113300(FSOverlayInfo *info);
void func_0211313c(FSOverlayInfo *info);

/* Reads an overlay into memory, then runs its static initialisers. */
BOOL FS_LoadOverlay(int target, u32 overlayId)
{
    FSOverlayInfo info;

    if (!func_021133a0(&info, target, overlayId) || !func_02113300(&info)) {
        return FALSE;
    }
    func_0211313c(&info);
    return TRUE;
}
