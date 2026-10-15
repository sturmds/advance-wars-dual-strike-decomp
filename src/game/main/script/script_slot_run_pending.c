#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void ScriptSlot_Run(int slot);

/* Runs the slot if bit 0 of unk_12 is set (see ScriptSlot_AnyPending), clearing the bit.
 * Calls the slot's callback first, with the slot as its argument; the callback field
 * is typed without the parameter, hence the cast. */
void ScriptSlot_RunPending(int slot) {
  ScriptSlot* target;

  if (!(data_027c027c->scriptSlots[slot].unk_12 & 1)) {
    return;
  }
  *data_027c027c->scriptSlotCurrent = slot;
  data_027c027c->scriptSlots[slot].unk_12 &= ~1;
  target = &data_027c027c->scriptSlots[slot];
  if (target->callback != NULL) {
    target->callback(target);
  }
  ScriptSlot_Run(slot);
}
