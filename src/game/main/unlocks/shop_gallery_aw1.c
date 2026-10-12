#include "types.h"
#include "game/unlocks.h"

BOOL GbaCart_IsAdvanceWars(void);

/* Battle Maps shop: Advance Wars gallery (bought flag 0x40), on sale while an
 * Advance Wars cartridge is in the GBA slot. */
int Shop_GalleryAw1(u32 unlockId) {
  if (GameFlag_IsSet(0x40)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GbaCart_IsAdvanceWars() ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
