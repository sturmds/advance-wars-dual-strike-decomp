#include "types.h"
#include "sdk/os.h"

int MATH_CountLeadingZeros(u32 value);   /* count leading zeros */

void OSi_UnlockVram(u16 banks, u16 lockID)
{
    u32 held;
    int irq = OS_DisableInterrupts();

    held = banks & gOsVramLockedBanks & VRAM_ALL_BANK_BITS;

    /* Visits the held banks from the highest bit down. */
    while (TRUE) {
        int bank = 31 - MATH_CountLeadingZeros(held);
        u32 keep;

        if (bank < 0) {
            break;
        }
        keep = ~(1 << bank);
        held &= keep;
        if (gOsVramLockIds[bank] == lockID) {
            gOsVramLockIds[bank] = 0;
            gOsVramLockedBanks &= keep;
        }
    }
    OS_RestoreInterrupts(irq);
}
