#include "types.h"
#include "sdk/ctrdg.h"

BOOL func_arm7_0380660c(void);

/* TRUE once the cartridge seen at start-up was pulled out; FALSE when there was none. */
BOOL CTRDG_IsPulledOut(void)
{
    GbaSlotBootSnapshot *info = GBA_SLOT_SNAPSHOT_PTR;

    if (info->reserved_be == 0xffff) {
        return FALSE;
    }
    if (!info->pulledOut) {
        func_arm7_0380660c();
    }
    return info->pulledOut;
}
