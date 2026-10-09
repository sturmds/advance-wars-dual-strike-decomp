#include "types.h"
#include "game/unlocks.h"

u8 func_02101924(u32 coId); /* tests CO coId's bit in data_022a7f80 (see CoColor_Set) */

/* Battle maps shop condition for a CO colour: returns -1 if already bought,
 * else 1 if CO coId is unlocked and game flag 0x23 (probably the crystal
 * smashed in the campaign) is set, else 0. */
int Shop_SmashedCrystal(u32 coId)
{
    if (func_02101924(coId)) {
        return -1;
    }
    /* needed to match: == 0, not ! */
    if (Unlock_IsSet(coId) == 0) {
        return 0;
    }
    return GameFlag_IsSet(0x23) != 0;
}
