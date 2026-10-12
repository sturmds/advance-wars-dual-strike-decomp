#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/script_slot.h"

extern Unk_027c0280* data_027c0280;
extern Unk_02080440* data_027c027c;

void Screen_ResetBlend(void);
void ScriptSlot_ClearCallback(int slot);

/* Script callback of the Start-button map overview: blend mode 3 darkens by
 * EVY, which counts down from the slot's unk_1e (set by MapOverview_Close) to
 * 0 (a fade in). At 0 it resets the blend and ends the callback. */
void MapOverview_FadeInStep(ScriptSlot* slot) {
  data_027c0280->blendMode = 3;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = slot->unk_1e;
  if (slot->unk_1e != 0) {
    slot->unk_1e--;
    return;
  }
  Screen_ResetBlend();
  ScriptSlot_ClearCallback(*data_027c027c->scriptSlotCurrent & 0xff);
}
