#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: the item goes on sale once Hard Campaign is beaten (flag 0x22). */
int Shop_BeatHardCampaign(u32 unlockId) {
  if (Unlock_IsSet(unlockId)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x22) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
