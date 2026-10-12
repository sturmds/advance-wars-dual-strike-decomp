#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: Oozium (bought flag 0x2b) goes on sale once the Black Obelisk is
 * destroyed (flag 0x24). */
int Shop_Oozium(u32 unlockId) {
  if (GameFlag_IsSet(0x2b)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x24) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
