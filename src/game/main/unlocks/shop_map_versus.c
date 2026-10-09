#include "types.h"
#include "game/unlocks.h"

u8 func_02101984(u32 mapId);

/* Battle Maps shop: a Versus map, always on sale until its map flag is set. */
int Shop_MapVersus(u32 unlockId)
{
    return func_02101984(unlockId) ? SHOP_ITEM_BOUGHT : SHOP_ITEM_FOR_SALE;
}
