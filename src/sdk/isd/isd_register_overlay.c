/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/fs.h"
#include "sdk/isd.h"

extern int (*gIsdRegisterOverlayHook)(MIProcessor target, void *address, u32 fileOffset, u32 size);

int func_02121424(MIProcessor target, void *address, u32 fileOffset, u32 size);

/* Tells the debugger that an overlay is loaded at address, and where its file lies in the ROM. */
int Isd_RegisterOverlay(MIProcessor target, void *address, u32 fileOffset, u32 size)
{
    IsdRegionState saved;
    int result;

    if (gIsdOverlayHooksEnabled) {
        Isd_OpenDebugRegion(&saved);
        result = gIsdRegisterOverlayHook(target, address, fileOffset, size);
        Isd_CloseDebugRegion(&saved);
        return result;
    }
    return func_02121424(target, address, fileOffset, size);
}
