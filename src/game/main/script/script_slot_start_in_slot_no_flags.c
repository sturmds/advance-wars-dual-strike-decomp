#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

void ScriptSlot_Run(int slot);
void ScriptSlot_Init(const ScriptCommand* script, u8 slot, int arg);

/* ScriptSlot_StartInSlot, except that unk_12 is cleared instead of set to 4. slot is
 * an s16 here; the call to ScriptSlot_Run narrows it to u8. */
s8 ScriptSlot_StartInSlotNoFlags(const ScriptCommand* script, s16 slot,
                                 int arg) {
  s16 saved = *data_027c027c->scriptSlotCurrent;

  *data_027c027c->scriptSlotCurrent = slot;
  ScriptSlot_Init(script, slot, arg);
  data_027c027c->scriptSlots[slot].unk_12 = 0;
  ScriptSlot_Run((u8)slot);
  *data_027c027c->scriptSlotCurrent = saved;
  return slot;
}
