#include "types.h"
#include "sdk/os.h"

void OSi_InitVramExclusive(void)
{
    int i;

    gOsVramLockedBanks = 0;
    for (i = 0; i < VRAM_LOCKABLE_BANKS; i++) {
        gOsVramLockIds[i] = 0;
    }
}
