#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: Hawke for Combat (bought flag 0x35) goes on sale once save flag 0x2a
 * is set. */
int Shop_CombatHawke(u32 unlockId) {
  if (GameFlag_IsSet(0x35)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x2a) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
