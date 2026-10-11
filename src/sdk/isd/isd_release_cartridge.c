#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

void func_0210cee4(int lockID);
void func_02121ac8(BOOL state);
void func_02121b14(BOOL state);

/* Undoes Isd_TryAcquireCartridge: frees the cartridge lock if we took it, then restores the two saved states. */
void Isd_ReleaseCartridge(u32 saved)
{
    if (saved & 0x20) {
        func_0210cee4(0x70);
    }
    func_02121ac8(saved & 2);
    func_02121b14(saved & 1);
}
