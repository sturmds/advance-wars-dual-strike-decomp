#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: bought flag 0x31; on sale once Money Survival is beaten (flag 0x2e). */
int Shop_BeatMoneySurvival(u32 unlockId) {
  if (GameFlag_IsSet(0x31)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x2e) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
