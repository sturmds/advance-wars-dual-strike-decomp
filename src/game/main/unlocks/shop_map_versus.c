#include "types.h"
#include "game/unlocks.h"

u8 Map_IsFlagSet(u32 mapId);

/* Battle Maps shop: a Versus map, always on sale until its map flag is set. */
int Shop_MapVersus(u32 unlockId) {
  return Map_IsFlagSet(unlockId) ? SHOP_ITEM_BOUGHT : SHOP_ITEM_FOR_SALE;
}
