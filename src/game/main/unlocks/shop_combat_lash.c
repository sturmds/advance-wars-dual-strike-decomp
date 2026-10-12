#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: Lash for Combat (bought flag 0x33) goes on sale once save flag 0x2a
 * is set. */
int Shop_CombatLash(u32 unlockId) {
  if (GameFlag_IsSet(0x33)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x2a) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
