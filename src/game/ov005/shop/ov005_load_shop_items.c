#include "types.h"
#include "game/play_records.h"
#include "game/Unk_ov005_0236bbb0.h"

int Ov005_CopyShopItemIfForSale(const ShopItem* src, ShopItem* dst);
void Ov005_InitShopIconBank(Unk_ov005_0236bbb0* work);
void Ov005_LoadShopIcon(Unk_ov005_0236bbb0* work, int iconId);

extern ShopItem data_0216cb60[];

/* Fills the shop list: takes the points from the play records, resets the
 * icon bank, collects up to shopItemCount items for sale from data_0216cb60
 * and stores how many there are. */
void Ov005_LoadShopItems(Unk_ov005_0236bbb0* work) {
  ShopItem* dst;
  const ShopItem* src;
  u32 count;

  work->items = work->itemBuf;
  dst = work->items;
  src = data_0216cb60;
  work->points = gPlayRecords.points;
  Ov005_InitShopIconBank(work);
  for (count = 0; count < work->shopItemCount; src++) {
    int result = Ov005_CopyShopItemIfForSale(src, dst);

    if (result == -1) {
      break;
    }
    if (result != 0) {
      Ov005_LoadShopIcon(work, dst->iconId);
      dst++;
      count++;
    }
  }
  work->itemCount = count;
}
