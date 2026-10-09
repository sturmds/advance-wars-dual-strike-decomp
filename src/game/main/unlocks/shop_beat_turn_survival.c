#include "types.h"
#include "game/unlocks.h"

BOOL func_02101744(int flag);

/* Battle Maps shop: bought flag 0x30; on sale once Turn Survival is beaten (flag 0x2d). */
int Shop_BeatTurnSurvival(u32 unlockId)
{
    if (func_02101744(0x30)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101744(0x2d) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
