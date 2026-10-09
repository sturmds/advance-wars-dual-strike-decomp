#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: Adder for Combat (bought flag 0x34) goes on sale once save flag 0x2a
 * is set. */
int Shop_CombatAdder(u32 unlockId)
{
    if (func_02101744(0x34)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x2a) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
