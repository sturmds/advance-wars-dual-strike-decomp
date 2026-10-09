#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: bought flag 0x2f; on sale once Time Survival is beaten (flag 0x2c). */
int Shop_BeatTimeSurvival(u32 unlockId)
{
    if (func_02101744(0x2f)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x2c) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
