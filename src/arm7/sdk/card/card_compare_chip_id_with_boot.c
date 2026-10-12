#include "types.h"
#include "sdk/card_arm7.h"

u32 Card_LockAndReadChipId(void);

/* Card removal check by chip ID: compares the ID read now with the one read at boot. */
BOOL Card_CompareChipIdWithBoot(void)
{
    volatile u32 *bootId;
    volatile u32 bootRomId;   /* needed to match: the original keeps it in a stack slot across the call */
    u32 romId;
    BOOL same;

    if (BOOT_CHIP_ID_COPY_SELECT == 0) {
        bootId = BOOT_CHIP_ID_FIRST_COPY;
    } else {
        bootId = BOOT_CHIP_ID_SECOND_COPY;
    }
    bootRomId = *bootId;
    romId = Card_LockAndReadChipId();
    same = (romId == bootRomId);
    gCardRemoved = !same;
    return same;
}
