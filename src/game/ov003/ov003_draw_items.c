#include "types.h"
#include "game/ov003_unit.h"
#include "game/ov003_item.h"
#include "game/ov003_marker.h"

Ov003Item data_ov003_02369b98[OV003_ITEM_COUNT];
Ov003Marker data_ov003_02369bb8[4];

void Ov003_DrawItem(Ov003Item* item);
int Ov003_IsPhase0OrAbove6(void);

/* Draws the live items. In its last 120 frames an item blinks: it is hidden on
 * frames with bit 1 of frameCount set, unless Ov003_IsPhase0OrAbove6 returns 0. */
void Ov003_DrawItems(void) {
  Ov003Item* item = data_ov003_02369b98;
  int i;

  for (i = 0; i < OV003_ITEM_COUNT; i++, item++) {
    if (item->timer == 0) {
      continue;
    }
    if (item->timer < 120 && (data_ov003_02364928->frameCount & 2) &&
        Ov003_IsPhase0OrAbove6() == 1) {
      continue;
    }
    Ov003_DrawItem(item);
  }
}
