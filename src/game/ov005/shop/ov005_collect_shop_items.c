#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

int Ov005_CopyShopItemIfForSale(const ShopItem* src, ShopItem* dst);
void Ov005_LoadShopIcon(Unk_ov005_0236bbb0* work, int iconId);

extern ShopItem data_0216cb60[];

/* Copies the items for sale from data_0216cb60 into work->items and loads
 * their icons, until shopItemCount are collected or the table ends. */
void Ov005_CollectShopItems(Unk_ov005_0236bbb0* work) {
  ShopItem* dst = work->items;
  const ShopItem* src = data_0216cb60;
  u32 count = 0;

  while (count < work->shopItemCount) {
    int result = Ov005_CopyShopItemIfForSale(src, dst);

    if (result == -1) {
      return;
    }
    if (result != 0) {
      Ov005_LoadShopIcon(work, dst->iconId);
      dst++;
      count++;
    }
    src++;
  }
}
