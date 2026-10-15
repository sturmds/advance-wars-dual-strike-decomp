#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/script_slot.h"

void ScriptSlot_ClearCallback(int slot);

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Script callback of the Start-button map overview: darkens the screen by
 * EVY = counter / 2 while the slot's counter (unk_1e) climbs to 12, so the
 * screen ends at 6/16 dark. Then it runs the screen config's unk_50 hook if
 * unk_48 is set and ends the callback. */
void MapOverview_DimStep(ScriptSlot* slot) {
  data_027c0280->blendMode = 3;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = slot->unk_1e >> 1;
  if (slot->unk_1e >= 12) {
    if (HOOKS->unk_48 != NULL) {
      HOOKS->unk_50();
    }
    ScriptSlot_ClearCallback(*data_027c027c->scriptSlotCurrent & 0xff);
  } else {
    slot->unk_1e++;
  }
}
