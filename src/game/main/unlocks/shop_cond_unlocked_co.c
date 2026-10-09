#include "types.h"
#include "game/unlocks.h"

u8 func_02101924(u32 coId); /* tests CO coId's bit in data_022a7f80 (see CoColor_Set) */

/* Battle maps shop condition for a CO colour: returns -1 if already bought,
 * else 1 if CO coId is unlocked, else 0. */
int ShopCond_UnlockedCo(u32 coId)
{
    if (func_02101924(coId)) {
        return -1;
    }
    return Unlock_IsSet(coId) != 0;
}
