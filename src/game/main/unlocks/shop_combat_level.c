#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: the Combat level (bought flag 0x29) goes on sale once save flag 0x2a
 * is set. */
int Shop_CombatLevel(u32 unlockId)
{
    if (func_02101744(0x29)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x2a) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
