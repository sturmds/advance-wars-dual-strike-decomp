#include "types.h"
#include "sdk/ctrdg.h"

void func_arm7_03806520(int lockID, GbaSlotLockRecord *lock);
void func_arm7_038064f8(int lockID, GbaSlotLockRecord *lock);
void func_arm7_038065c4(GbaSlotWaitStates *saved);
void func_arm7_03806590(GbaSlotWaitStates *timing);

/* ARM7 side state of the library. */
extern CTRDGWork data_arm7_0380afc0;

/* ARM7 copy of Cartridge_HeaderStillMatches. */
BOOL func_arm7_0380660c(void)
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
    func_arm7_03806520(data_arm7_0380afc0.lockID, &lock);
    func_arm7_038065c4(&timing);
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
    func_arm7_03806590(&timing);
    func_arm7_038064f8(data_arm7_0380afc0.lockID, &lock);
    return unchanged;
}
