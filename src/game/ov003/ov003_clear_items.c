#include "types.h"
#include "game/ov003_item.h"

void Ov003_ClearItems(void) {
  Ov003Item* item = data_ov003_02369b98;
  int i;

  for (i = 0; i < OV003_ITEM_COUNT; i++, item++) {
    item->kind = 0;
    item->timer = 0;
  }
}
