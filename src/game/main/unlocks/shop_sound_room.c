#include "types.h"
#include "game/unlocks.h"

/* Battle Maps shop: the Sound Room (bought flag 0x26) goes on sale once the Campaign is
 * beaten (flag 0x21). */
int Shop_SoundRoom(u32 unlockId) {
  if (GameFlag_IsSet(0x26)) {
    return SHOP_ITEM_BOUGHT;
  }
  return GameFlag_IsSet(0x21) ? SHOP_ITEM_FOR_SALE : SHOP_ITEM_LOCKED;
}
