#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: Flak for Combat (bought flag 0x32) goes on sale once save flag 0x2a
 * is set. */
int Shop_CombatFlak(u32 unlockId) {
  if (GameFlag_IsSet(0x32)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x2a) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
