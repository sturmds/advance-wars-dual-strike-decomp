#include "types.h"
#include "game/unlocks.h"

BOOL GbaCart_IsAdvanceWars2(void);

/* Battle Maps shop: Advance Wars 2 gallery (bought flag 0x41), on sale while an
 * Advance Wars 2 cartridge is in the GBA slot. */
int Shop_GalleryAw2(u32 unlockId) {
  if (GameFlag_IsSet(0x41)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GbaCart_IsAdvanceWars2() ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
