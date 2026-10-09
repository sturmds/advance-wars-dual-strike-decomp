#include "types.h"
#include "game/unlocks.h"

u8 func_02101984(u32 mapId);
BOOL GbaCart_IsAdvanceWars(void);

/* Battle Maps shop: an Advance Wars map, on sale while an Advance Wars cartridge
 * is in the GBA slot. Bought once its map flag is set. */
int Shop_MapAw1(u32 unlockId)
{
    if (func_02101984(unlockId)) {
        return SHOP_ITEM_BOUGHT;
    }
    return GbaCart_IsAdvanceWars() ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
