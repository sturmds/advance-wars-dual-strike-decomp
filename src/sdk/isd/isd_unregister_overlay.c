/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/fs.h"
#include "sdk/isd.h"

extern int (*gIsdUnregisterOverlayHook)(MIProcessor target, void *address, u32 size);

int func_021210d8(MIProcessor target, void *address, u32 size);

/* Tells the debugger that an overlay's memory range is no longer loaded. */
int Isd_UnregisterOverlay(MIProcessor target, void *address, u32 size)
{
    IsdRegionState saved;
    int result;

    if (gIsdOverlayHooksEnabled) {
        Isd_OpenDebugRegion(&saved);
        result = gIsdUnregisterOverlayHook(target, address, size);
        Isd_CloseDebugRegion(&saved);
        return result;
    }
    return func_021210d8(target, address, size);
}
