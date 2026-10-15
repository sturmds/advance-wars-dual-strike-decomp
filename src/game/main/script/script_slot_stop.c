#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

void func_02015ef8(int slot);
void func_0201751c(int arg);

/* Frees the slot. Calls its onStop handler first, and, when bit 2 of unk_12 is
 * set, the two functions that follow (both empty in this build). The current
 * slot number is saved and restored around the calls. */
void ScriptSlot_Stop(int slot) {
  s16 saved = *data_027c027c->scriptSlotCurrent;

  data_027c027c->scriptSlots[slot].start = NULL;
  if (data_027c027c->scriptSlots[slot].onStop != NULL) {
    *data_027c027c->scriptSlotCurrent = slot;
    data_027c027c->scriptSlots[slot].onStop(&data_027c027c->scriptSlots[slot]);
  }
  if (data_027c027c->scriptSlots[slot].unk_12 & 2) {
    func_02015ef8(slot);
    func_0201751c(data_027c027c->scriptSlots[slot].unk_26);
  }
  *data_027c027c->scriptSlotCurrent = saved;
}
