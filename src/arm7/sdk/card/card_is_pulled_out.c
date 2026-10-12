#include "types.h"
#include "sdk/card_arm7.h"

/*
 * TRUE once the game card was found removed. Until then each call checks
 * again, by chip ID or by interrupt as the boot info asks.
 */
BOOL CARD_IsPulledOut(void)
{
    if (!gCardRemoved) {
        if (BOOT_REMOVAL_TEST_MODE & REMOVAL_TEST_USES_CHIP_ID) {
            Card_CompareChipIdWithBoot();
        } else {
            Card_TestRemovalIrqFlag();
        }
    }
    return gCardRemoved;
}
