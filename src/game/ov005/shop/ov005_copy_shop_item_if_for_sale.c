#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

void Mem_Copy32(const void* src, void* dst, u32 size);

/* Copies a table entry to dst when its isForSale check passes. Returns -1 at
 * the end of the table, 0 when the item is not for sale, 1 when copied. */
int Ov005_CopyShopItemIfForSale(const ShopItem* src, ShopItem* dst) {
  if (src->price == -1) {
    return -1;
  }
  if (src->isForSale(src->arg) != 1) {
    return 0;
  }
  Mem_Copy32(src, dst, sizeof(ShopItem));
  return 1;
}
