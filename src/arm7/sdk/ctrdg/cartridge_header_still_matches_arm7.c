#include "types.h"
#include "sdk/ctrdg.h"

/* ARM7 side state of the library. */
extern CTRDGWork data_arm7_0380afc0;

/* ARM7 copy of Cartridge_HeaderStillMatches. */
BOOL Cartridge_HeaderStillMatches_Arm7(void)
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
    Cartridge_SpinUntilSlotOwned_Arm7(data_arm7_0380afc0.lockID, &lock);
    Cartridge_StashAndSlowRomWaits_Arm7(&timing);
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
        info->pulledOut = 1;
        unchanged = FALSE;
    }
    Cartridge_RestoreRomWaits_Arm7(&timing);
    Cartridge_ReleaseSlotIfTaken_Arm7(data_arm7_0380afc0.lockID, &lock);
    return unchanged;
}
