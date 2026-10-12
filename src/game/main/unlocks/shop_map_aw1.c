#include "types.h"
#include "game/unlocks.h"

u8 Map_IsFlagSet(u32 mapId);
BOOL GbaCart_IsAdvanceWars(void);

/* Battle Maps shop: an Advance Wars map, on sale while an Advance Wars cartridge
 * is in the GBA slot. Bought once its map flag is set. */
int Shop_MapAw1(u32 unlockId) {
  if (Map_IsFlagSet(unlockId)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GbaCart_IsAdvanceWars() ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
