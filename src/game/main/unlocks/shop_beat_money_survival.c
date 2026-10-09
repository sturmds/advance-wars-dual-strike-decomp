#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: bought flag 0x31; on sale once Money Survival is beaten (flag 0x2e). */
int Shop_BeatMoneySurvival(u32 unlockId)
{
    if (func_02101744(0x31)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x2e) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
