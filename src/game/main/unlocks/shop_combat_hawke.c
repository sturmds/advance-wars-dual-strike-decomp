#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: Hawke for Combat (bought flag 0x35) goes on sale once save flag 0x2a
 * is set. */
int Shop_CombatHawke(u32 unlockId)
{
    if (func_02101744(0x35)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x2a) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
