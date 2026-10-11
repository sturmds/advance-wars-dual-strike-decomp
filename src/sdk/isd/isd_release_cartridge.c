#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

void func_0210cee4(int lockID);
void Isd_RestoreFiq(BOOL state);
void Isd_RestoreIrq(BOOL state);

/* Undoes Isd_TryAcquireCartridge: frees the cartridge lock if we took it, then restores the two saved states. */
void Isd_ReleaseCartridge(u32 saved)
{
    if (saved & 0x20) {
        func_0210cee4(0x70);
    }
    Isd_RestoreFiq(saved & 2);
    Isd_RestoreIrq(saved & 1);
}
