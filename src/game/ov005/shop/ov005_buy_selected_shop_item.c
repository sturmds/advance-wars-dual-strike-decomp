#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

int Ov005_GetSelectedShopItemIndex(Unk_ov005_0236bbb0* work);

/* Runs the buy callback of the item under the cursor. */
void Ov005_BuySelectedShopItem(Unk_ov005_0236bbb0* work) {
  ShopItem* item = &work->items[Ov005_GetSelectedShopItemIndex(work)];

  item->onBuy(item->arg, 1);
}
