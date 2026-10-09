#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: Oozium (bought flag 0x2b) goes on sale once the Black Obelisk is
 * destroyed (flag 0x24). */
int Shop_Oozium(u32 unlockId)
{
    if (func_02101744(0x2b)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x24) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
