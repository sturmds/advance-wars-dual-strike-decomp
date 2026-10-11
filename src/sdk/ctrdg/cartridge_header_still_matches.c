#include "types.h"
#include "sdk/ctrdg.h"

BOOL Cartridge_HeaderStillMatches(void)
{
    GbaSlotLockRecord lock;
    GbaSlotWaitStates timing;
    BOOL unchanged = TRUE;
    GbaSlotBootSnapshot *info = GBA_SLOT_SNAPSHOT_PTR;
    GbaRomHeader *header;
    u8 fixedValue;

    if (info->reserved_be == 0xffff) {
        return FALSE;
    }
    if (info->pulledOut == 1) {
        return FALSE;
    }
    Cartridge_SpinUntilSlotOwned(data_022acf14.lockID, &lock);
    Cartridge_StashAndSlowRomWaits(&timing);
    header = GBA_SLOT_HEADER_PTR;
    fixedValue = header->fixedValue;
    if (fixedValue == GBA_HEADER_MAGIC_BYTE) {
        if (info->reserved_be != header->reserved_be) {
            goto changed;
        }
    }
    if (fixedValue != GBA_HEADER_MAGIC_BYTE) {
        if (info->reserved_be != *GBA_ROM_128K_LAST_HALFWORD) {
            goto changed;
        }
    }
    if (info->gameCode != header->gameCode && info->isAgb) {
changed:
        unchanged = FALSE;
        info->pulledOut = 1;
    }
    Cartridge_RestoreRomWaits(&timing);
    Cartridge_ReleaseSlotIfTaken(data_022acf14.lockID, &lock);
    return unchanged;
}
