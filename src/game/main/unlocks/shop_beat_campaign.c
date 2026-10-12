#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: the item goes on sale once the Campaign is beaten (flag 0x21). */
int Shop_BeatCampaign(u32 unlockId) {
  if (Unlock_IsSet(unlockId)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x21) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
