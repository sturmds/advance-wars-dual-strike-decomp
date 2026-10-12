#include "types.h"
#include "game/ov003_item.h"

s32 HwMod32(s32 numer, s32 denom);
u32 Ov003_RandNext(void);

/* Puts an item at (x, y) in a free entry. Returns 0 when none is free or on a
 * 1 in 6 chance; otherwise the random roll (1 to 5) is the item kind. */
int Ov003_SpawnItem(int x, int y) {
  Ov003Item* item = data_ov003_02369b98;
  int i;
  int kind;

  for (i = 0; i < OV003_ITEM_COUNT; i++, item++) {
    if (item->timer == 0) {
      break;
    }
  }
  if (i == OV003_ITEM_COUNT) {
    return 0;
  }
  kind = HwMod32(Ov003_RandNext(), 6);
  if (kind == 0) {
    return 0;
  }
  item->x = x;
  item->y = y;
  item->kind = kind;
  item->timer = 600;
  return 1;
}
