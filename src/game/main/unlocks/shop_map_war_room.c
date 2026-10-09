#include "types.h"
#include "game/unlocks.h"

u8 func_02101984(u32 mapId);
u8 func_02101964(u32 id);

/* Battle Maps shop: a War Room map, on sale once flag 7 of the second flag set at
 * data_022a7f58 is set. Bought once its map flag is set. */
int Shop_MapWarRoom(u32 unlockId)
{
    if (func_02101984(unlockId)) {
        return SHOP_ITEM_BOUGHT;
    }
    return func_02101964(7) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
