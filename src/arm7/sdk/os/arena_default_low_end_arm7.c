#include "types.h"
#include "sdk/os_arm7.h"

extern u8 data_arm7_027d84fc[];    /* end of the ARM7 code and data in main RAM */
extern u8 data_arm7_0380afec[];    /* end of the ARM7 code and data in WRAM */

#define ARM7_OWN_WRAM_BASE 0x03800000 /* ARM7's own WRAM starts here; shared WRAM ends here */

/* ARM7 copy of OS_GetInitArenaLo. Initial low end of each arena; NULL for an id with no arena. */
void *Arena_DefaultLowEnd_Arm7(int id)
{
    u32 lo;

    switch (id) {
    case OS_ARENA_MAIN_SUBPRIV:
        return data_arm7_027d84fc;
    case OS_ARENA_WRAM_SUB:
        lo = (u32)data_arm7_0380afec;
        if (lo > ARM7_OWN_WRAM_BASE) {
            lo = ARM7_OWN_WRAM_BASE;
        }
        return (void *)lo;
    case OS_ARENA_WRAM_SUBPRIV:
        lo = ARM7_OWN_WRAM_BASE;
        if ((u32)data_arm7_0380afec > ARM7_OWN_WRAM_BASE) {
            lo = (u32)data_arm7_0380afec;
        }
        return (void *)lo;
    default:
        return NULL;
    }
}
